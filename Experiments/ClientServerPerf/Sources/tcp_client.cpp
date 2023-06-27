//
// Application to make statistics on tcp
// inspired by blocking_tcp_echo_client.cpp (from https://www.boost.org/doc/libs/1_35_0/doc/html/boost_asio/example/echo/blocking_tcp_echo_client.cpp)
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//

#include <iostream>
#include <thread>
#include <vector>
#include <boost/asio.hpp>
#include <numeric>
#include "common.h"
#include "options.h"
#include "options_ext.h"

using boost::asio::ip::tcp;
using namespace std;
using mlib::OptParser;

// The following value has been found experimentally when filler field of ClientMessageToBroadcast has size 0
constexpr int minSizeClientMessageToBroadcast = 22;

struct measures_t {
    explicit measures_t(size_t nb_rtts_max)
    : rtts(nb_rtts_max)
    {
    }
    vector<std::chrono::duration<double, std::milli>> rtts; // Round-Trip Time
    atomic_size_t nb_rtts{0};
};

struct param_t {
    string host;
    string port;
    int nb_messages{0};
    chrono::duration<int64_t, ratio<1, 1000>> send_interval{0};
    int size_messages{0};
    int nb_clients{0};
    bool verbose{false};
};

// Should return true if analyze_packet discovers client will not receive any more packets.
bool analyze_packet(string_view msg_sv, unsigned char& myId, param_t const& param, measures_t & measures)
{
    std::string msg_string{msg_sv};
    std::istringstream msg_stream{ msg_string };
    unsigned char msg_id;
    msg_stream >> msg_id;
    switch (ServerMsgId server_msg_typ{msg_id }; server_msg_typ)
    {
        case ServerMsgId::AckDoneSendingMessages :
            return true;
        case ServerMsgId::BroadcastMessage :
        {
            auto sbm {read_data_in_msg_stream<ServerBroadcastMessage>(msg_stream)};
            chrono::duration<double, std::milli> elapsed = std::chrono::system_clock::now() - sbm.sendTime;
            if (sbm.senderId == myId)
                measures.rtts[measures.nb_rtts++] = elapsed;
            if (param.verbose) {
                cout << "Client #" << static_cast<unsigned int>(myId) << " : ";
                cout << "Received broadcast message #" << sbm.messageId << " echoed from ";
                if (sbm.senderId == myId)
                    cout << "myself";
                else
                    cout << "client " << static_cast<unsigned int>(sbm.senderId);
                // C++20 operator<< is not yet implemented for duration in gcc.
                // Thus, we cannot write: cout << " in " << elapsed << "\n";
                // ==> We do it manually.
                cout << " in " << elapsed.count() << " ms\n";
            }
            return false;
        }
        case ServerMsgId::IdResponse :
        {
            auto sir {read_data_in_msg_stream<ServerIdResponse>(msg_stream)};
            myId = sir.id;
            return false;
        }
        default:
            return false;
    }
}

void msg_receive(tcp::socket &s, unsigned char &myId, param_t const& param, measures_t & measures)
{
    for (;;)
    {
        size_t len;
        size_t len_length = boost::asio::read(s,
                                                boost::asio::buffer(&len, sizeof(len)));
        assert(len_length == sizeof(len));
        char msg[max_length];
        size_t msg_length = boost::asio::read(s,
                                                boost::asio::buffer(msg, len));
        assert(msg_length == len);
        std::string_view msg_sv{msg, len};
        if (analyze_packet(msg_sv, myId, param, measures))
            break;
    }
}

void client(param_t const& param, measures_t & measures)
{
    try
    {
        // EndPoint creation
        boost::asio::io_service io_service;

        tcp::resolver resolver(io_service);
        tcp::resolver::query query(tcp::v4(), param.host, param.port);
        tcp::resolver::iterator iterator = resolver.resolve(query);

        tcp::socket s(io_service);
        s.connect(*iterator);

        boost::asio::ip::tcp::no_delay option(true);
        s.set_option(option);

        unsigned char myId{ 0 };

        // Create a thread for receiving data
        auto t = jthread(msg_receive, std::ref(s), std::ref(myId), std::ref(param), std::ref(measures));

        // Send IdRequest to server
        auto s_cir {prepare_msg_with_no_data<ClientMsgId>(ClientMsgId::IdRequest)};
        auto len = s_cir.length();
        boost::asio::write(s, boost::asio::buffer(&len, sizeof(len)));
        boost::asio::write(s, boost::asio::buffer(s_cir.data(), len));

        // Wait for IdResponse from server (received by msg_receive thread)
        while (myId == 0)
        {
            this_thread::sleep_for(50ms);
        }

        if (param.verbose)
            cout << "Received my Id which is: " << static_cast<unsigned int>(myId) << "\n";

        for (unsigned int i = 0; i < param.nb_messages; ++i) {
            if (param.verbose)
                cout << "Request to broadcast message #" << i << "\n";
            auto s_cmtb {prepare_msg<ClientMsgId, ClientMessageToBroadcast>(ClientMsgId::MessageToBroadcast,
                                                                       ClientMessageToBroadcast{ myId, i, std::chrono::system_clock::now(), std::string(param.size_messages - minSizeClientMessageToBroadcast, 0) })};

            len = s_cmtb.length();
            boost::asio::write(s, boost::asio::buffer(&len, sizeof(len)));
            boost::asio::write(s, boost::asio::buffer(s_cmtb.data(), len));

            this_thread::sleep_for(param.send_interval);
        }

        // Send DoneSendingMessages to server
        auto s_cdsm {prepare_msg<ClientMsgId, ClientDoneSendingMessages>(ClientMsgId::DoneSendingMessages,
                                                                        ClientDoneSendingMessages{ myId })};
        len = s_cdsm.length();
        boost::asio::write(s, boost::asio::buffer(&len, sizeof(len)));
        boost::asio::write(s, boost::asio::buffer(s_cdsm.data(), len));

        // msg_receive thread will exit when it has received ServerMsgId::AckDoneSendingMessages)
        t.join();

        if (param.verbose)
            cout << "Client #" << static_cast<unsigned int>(myId) << " done\n";
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }
}

struct param_t getParam(int argc, char* argv[])
{
    OptParser parser{
            "h|help \t Show help message",
            "H:host hostname \t Host (or IP address) to connect to",
            "p:port port_number \t Port to connect to",
            "n:nb_messages number \t Number of messages to be sent",
            "i:interval time_in_milliseconds \t Time interval between two sending of messages by a single client",
            "s:size size_in_bytes \t Size of messages sent by a client (min is 22 ==> If lower than 22, will be set to 22)",
            "c:clients number \t Number of clients which send messages to server",
            "v|verbose \t [optional] Verbose display required"
    };

    int nonopt;
    if (int ret ; (ret = parser.parse (argc, argv, &nonopt)) != 0)
    {
        if (ret == 1)
            cout << "Unknown option: " << argv[nonopt] << " Valid options are : " << endl
                 << parser.synopsis () << endl;
        else if (ret == 2)
            cout << "Option " << argv[nonopt] << " requires an argument." << endl;
        else if (ret == 3)
            cout << "Invalid options combination: " << argv[nonopt] << endl;
        exit (1);
    }
    if ((argc == 1) || parser.hasopt ('h'))
    {
        //No arguments on command line or help required. Show help and exit.
        cout << "Usage:" << endl;
        cout << parser.synopsis () << endl;
        cout << "Where:" << endl
             << parser.description () << endl;
        exit (0);
    }

    struct param_t param{
            getopt_required_string(parser, 'H'),
            getopt_required_string(parser, 'p'),
            getopt_required_int(parser, 'n'),
            chrono::milliseconds(getopt_required_int(parser, 'i')),
            getopt_required_int(parser, 's'),
            getopt_required_int(parser, 'c'),
            parser.hasopt ('v')
    };
    param.size_messages = (param.size_messages >= minSizeClientMessageToBroadcast ? param.size_messages : minSizeClientMessageToBroadcast);

    if (nonopt < argc)
        cout << "WARNING: There is a non-option argument: " << argv[nonopt] << " ==> It won't be used" << endl;

    return param;
}

int main(int argc, char* argv[])
{
    struct param_t param{getParam(argc, argv)};

    // Variables used during experiment
    measures_t measures(param.nb_messages * param.nb_clients);

    // We launch all of the clients to make the experiment
    vector<jthread> clients(param.nb_clients);
    for (auto& c : clients) {
        c = jthread(client, std::ref(param), std::ref(measures));
    }
    for (auto& c: clients)
        c.join();

    // Display statistics
    measures.rtts.resize(measures.nb_rtts);
    std::ranges::sort(measures.rtts);
    cout << "Ratio received / sent messages = " << measures.nb_rtts << " / " << param.nb_messages * param.nb_clients << " (" << measures.nb_rtts * 100 / param.nb_messages / param.nb_clients << "%)\n";
    cout << "Average = " << (std::reduce(measures.rtts.begin(), measures.rtts.end()) / measures.rtts.size()).count() << " ms\n";
    cout << "Min = " << measures.rtts[0].count() << " ms\n";
    cout << "Q1 = " << measures.rtts[measures.rtts.size()/4].count() << " ms\n";
    cout << "Q2 = " << measures.rtts[measures.rtts.size()/2].count() << " ms\n";
    cout << "Q3 = " << measures.rtts[measures.rtts.size()*3/4].count() << " ms\n";
    cout << "Centile #99 = " << measures.rtts[measures.rtts.size()*99/100].count() << " ms\n";
    cout << "Max = " << measures.rtts[measures.rtts.size()-1].count() << " ms\n";

    return 0;
}
