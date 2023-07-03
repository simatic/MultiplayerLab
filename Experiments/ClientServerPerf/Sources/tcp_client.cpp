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
#include <latch>
#include "common.h"
#include "options.h"
#include "OptParser_ext.h"
#include "tcp_communication.h"

using boost::asio::ip::tcp;
using namespace std;
using namespace mlib;

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
bool analyze_packet(string_view msg_sv, unsigned char& myId, param_t const& param, measures_t & measures, std::latch &broadcast_begin, std::latch &broadcast_end)
{
    std::string msg_string{msg_sv};
    std::istringstream msg_stream{ msg_string };
    unsigned char msg_id;
    msg_stream >> msg_id;
    switch (ServerMsgId server_msg_typ{msg_id }; server_msg_typ)
    {
        case ServerMsgId::AckDisconnectIntent :
            return true;
        case ServerMsgId::AckDoneSendingMessages :
            return false;
        case ServerMsgId::BroadcastBegin :
            broadcast_begin.count_down();
            return false;
        case ServerMsgId::BroadcastMessage :
        {
            auto sbm {read_data_in_msg_stream<ServerBroadcastMessage>(msg_stream)};
            chrono::duration<double, std::milli> elapsed = std::chrono::system_clock::now() - sbm.sendTime;
            if (sbm.senderId == myId)
            {
                measures.rtts[measures.nb_rtts++] = elapsed;
            }
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
        case ServerMsgId::BroadcastEnd :
            broadcast_end.count_down();
            return false;
        case ServerMsgId::IdResponse :
        {
            auto sir {read_data_in_msg_stream<ServerIdResponse>(msg_stream)};
            myId = sir.id;
            return false;
        }
        default:
        {
            cerr << "ERROR : Unexpected server_msg_typ (" << static_cast<int>(server_msg_typ) << ")\n";
            exit(EXIT_FAILURE);
        }
    }
}

void msg_receive(tcp::socket &sock, unsigned char &myId, param_t const& param, measures_t & measures, std::latch &broadcast_begin, std::latch &broadcast_end)
{
    for (;;)
    {
        auto msg_s{tcp_receive(&sock)};
        if (analyze_packet(msg_s, myId, param, measures, broadcast_begin, broadcast_end))
            break;
    }
}

void client(param_t const& param, measures_t & measures, std::latch &broadcast_begin, std::latch &broadcast_end)
{
    try
    {
        // EndPoint creation
        boost::asio::io_service io_service;

        tcp::resolver resolver(io_service);
        tcp::resolver::query query(tcp::v4(), param.host, param.port);
        tcp::resolver::iterator iterator = resolver.resolve(query);

        tcp::socket sock(io_service);
        sock.connect(*iterator);

        boost::asio::ip::tcp::no_delay option(true);
        sock.set_option(option);

        unsigned char myId{ 0 };

        // Create a thread for receiving data
        auto t = jthread(msg_receive, std::ref(sock), std::ref(myId), std::ref(param), std::ref(measures), std::ref(broadcast_begin), std::ref(broadcast_end));

        // Send IdRequest to server
        auto s_cir {prepare_msg<ClientMsgId, ClientIdRequest>(ClientMsgId::IdRequest, ClientIdRequest{param.nb_clients})};
        tcp_send(&sock, s_cir);

        broadcast_begin.wait();

        if (param.verbose)
            cout << "My Id which is: " << static_cast<unsigned int>(myId) << " and I can start broadcasting\n";

        for (unsigned int i = 0; i < param.nb_messages; ++i) {
            if (param.verbose)
                cout << "Request to broadcast message #" << i << "\n";
            auto s_cmtb {prepare_msg<ClientMsgId, ClientMessageToBroadcast>(ClientMsgId::MessageToBroadcast,
                                                                       ClientMessageToBroadcast{ myId, i, std::chrono::system_clock::now(), std::string(param.size_messages - minSizeClientMessageToBroadcast, 0) })};
            tcp_send(&sock, s_cmtb);
            this_thread::sleep_for(param.send_interval);
        }

        // Send DoneSendingMessages to server
        auto s_cdsm {prepare_msg<ClientMsgId, ClientDoneSendingMessages>(ClientMsgId::DoneSendingMessages,
                                                                        ClientDoneSendingMessages{ myId })};
        tcp_send(&sock, s_cdsm);

        broadcast_end.wait();

        // Send DisconnectIntent to server
        auto s_cdi {prepare_msg<ClientMsgId, ClientDisconnectIntent>(ClientMsgId::DisconnectIntent,
                                                                         ClientDisconnectIntent{ myId })};
        tcp_send(&sock, s_cdi);

        // msg_receive thread will exit when it has received ServerMsgId::AckDisconnectIntent)
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
    OptParser_ext parser{
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
            parser.getopt_required_string('H'),
            parser.getopt_required_string('p'),
            parser.getopt_required_int('n'),
            chrono::milliseconds(parser.getopt_required_int('i')),
            parser.getopt_required_int('s'),
            parser.getopt_required_int('c'),
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
    std::latch broadcast_begin(param.nb_clients);
    std::latch broadcast_end(param.nb_clients);

    // We launch all of the clients to make the experiment
    vector<jthread> clients(param.nb_clients);
    for (auto& c : clients) {
        c = jthread(client, std::ref(param), std::ref(measures), std::ref(broadcast_begin), std::ref(broadcast_end));
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
