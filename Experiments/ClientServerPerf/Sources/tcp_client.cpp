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

using boost::asio::ip::tcp;
using namespace std;

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
    bool verbose{false};
    int nb_messages{0};
    chrono::duration<int64_t, ratio<1, 1000>> send_interval{0};
    int size_messages{0};
    int nb_clients{0};
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
                cout << "ClientMsgId " << static_cast<unsigned int>(myId) << " : ";
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
                                                                       ClientMessageToBroadcast{ myId, i, std::chrono::system_clock::now() })};

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

        cout << "ClientMsgId " << static_cast<unsigned int>(myId) << " done\n";
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }
}

void usage()
{
    cerr << "Usage: tcp_client <host> <port> <verbose_0_or_1> <nb_messages> <send_interval_milliseconds> <size_messages> <nb_clients>\n";
}

int main(int argc, char* argv[])
{
    struct param_t param;

    if (argc != 8)
    {
        usage();
        return EXIT_FAILURE;
    }

    // Read arguments
    param.host = argv[1];

    param.port = argv[2];

    param.verbose = (argv[3][0] == '1');

    std::istringstream iss4(argv[4]);
    iss4 >> param.nb_messages;
    if (!iss4)
    {
        usage();
        return EXIT_FAILURE;
    }

    std::istringstream iss5(argv[5]);
    long long nb_milli;
    iss5 >> nb_milli;
    if (!iss5)
    {
        usage();
        return EXIT_FAILURE;
    }
    param.send_interval = chrono::milliseconds(nb_milli);

    std::istringstream iss6(argv[6]);
    iss6 >> param.size_messages;
    if (!iss6)
    {
        usage();
        return EXIT_FAILURE;
    }

    std::istringstream iss7(argv[7]);
    iss7 >> param.nb_clients;
    if (!iss7)
    {
        usage();
        return EXIT_FAILURE;
    }

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
