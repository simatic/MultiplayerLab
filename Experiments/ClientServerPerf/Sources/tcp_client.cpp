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

using StatTable=vector<std::chrono::duration<double, std::milli>>;

void analyze_packet(string_view msg_sv, unsigned char& myId, const bool verbose, StatTable &samples, atomic_size_t & nb_msg_received)
{
    std::string msg_string{msg_sv};
    std::istringstream msg_stream{ msg_string };
    unsigned char msg_typ;
    msg_stream >> msg_typ;
    switch (Server server_msg_typ{ msg_typ }; server_msg_typ)
    {
        case Server::IdResponse :
            struct ServerIdResponse sir;
            {
                cereal::BinaryInputArchive iarchive(msg_stream); // Create an input archive
                iarchive(sir); // Read the data from the archive
            }
            myId = sir.id;
            break;
        case Server::BroadcastMessage :
            struct ServerBroadcastMessage sbm;
            {
                cereal::BinaryInputArchive iarchive(msg_stream); // Create an input archive
                iarchive(sbm); // Read the data from the archive
            }
            chrono::duration<double, std::milli> elapsed = std::chrono::system_clock::now() - sbm.sendTime;
            samples[nb_msg_received++] = elapsed;
            if (verbose)
            {
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
            break;
    }
}

[[noreturn]]void msg_receive(tcp::socket &s, unsigned char &myId, const bool verbose, StatTable &samples, atomic_size_t & nb_msg_received)
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
        analyze_packet(msg_sv, myId, verbose, samples, nb_msg_received);
    }
}

void usage()
{
    cerr << "Usage: tcp_client <host> <port> <verbose_0_or_1> <nb_messages> <send_interval_milliseconds> <size_messages>\n";
}

int main(int argc, char* argv[])
{
    if (argc != 7)
    {
        usage();
        return EXIT_FAILURE;
    }

    // Read arguments
    string host{argv[1]};

    string port{argv[2]};

    bool verbose = (argv[3][0] == '1');

    std::istringstream iss4(argv[4]);
    int nb_messages;
    iss4 >> nb_messages;
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
    auto send_interval{ chrono::milliseconds(nb_milli)};

    std::istringstream iss6(argv[6]);
    int size_messages;
    iss6 >> size_messages;
    if (!iss6)
    {
        usage();
        return EXIT_FAILURE;
    }

    // Variables used during experiment
    unsigned char myId{ 0 };
    StatTable samples(nb_messages);
    atomic_size_t nb_msg_received{0};

    try
    {
        // EndPoint creation
        boost::asio::io_service io_service;

        tcp::resolver resolver(io_service);
        tcp::resolver::query query(tcp::v4(), host, port);
        tcp::resolver::iterator iterator = resolver.resolve(query);

        tcp::socket s(io_service);
        s.connect(*iterator);

        boost::asio::ip::tcp::no_delay option(true);
        s.set_option(option);

        // Create a thread for receiving data
        auto t = jthread(msg_receive, std::ref(s), std::ref(myId), verbose, std::ref(samples), std::ref(nb_msg_received));
        t.detach();

        // Send IdRequest to server
        stringstream ir_stream;
        ir_stream << static_cast<unsigned char>(Client::IdRequest);
        std::string_view ir_sv{ ir_stream.view() };
        size_t len = ir_sv.length();
        boost::asio::write(s, boost::asio::buffer(&len, sizeof(len)));
        boost::asio::write(s, boost::asio::buffer(ir_sv.data(), len));

        // Wait for IdResponse from server (received by msg_receive thread)
        while (myId == 0)
        {
              this_thread::sleep_for(50ms);
          }

        if (verbose)
            cout << "Received my Id which is: " << static_cast<unsigned int>(myId) << "\n";

        for (unsigned int i = 0; i < nb_messages; ++i) {
            if (verbose)
                cout << "Request to broadcast message #" << i << "\n";
            std::stringstream cmtb_stream;
            cmtb_stream << static_cast<unsigned char>(Client::MessageToBroadcast);
            {
                cereal::BinaryOutputArchive oarchive(cmtb_stream); // Create an output archive
                struct ClientMessageToBroadcast cmtb { myId, i, std::chrono::system_clock::now() };
                oarchive(cmtb); // Write the data to the archive
            } // archive goes out of scope, ensuring all contents are flushed
            std::string_view cmtb_sv{ cmtb_stream.view() };
            len = cmtb_sv.length();
            boost::asio::write(s, boost::asio::buffer(&len, sizeof(len)));
            boost::asio::write(s, boost::asio::buffer(cmtb_sv.data(), len));

            this_thread::sleep_for(send_interval);
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    // Display statistics
    samples.resize(nb_msg_received);
    ranges::sort(samples);
    cout << "Ratio received / sent messages = " << nb_msg_received << " / " << nb_messages << " (" << nb_msg_received * 100 / nb_messages << "%)\n";
    std::chrono::duration<double, std::milli> sum{0.0};
    for (auto const& s: samples)
        sum += s;
    cout << "Average = " << sum.count() / samples.size() << " ms\n";
    cout << "Min = " << samples[0].count() << " ms\n";
    cout << "Q1 = " << samples[samples.size()/4].count() << " ms\n";
    cout << "Q2 = " << samples[samples.size()/2].count() << " ms\n";
    cout << "Q3 = " << samples[samples.size()*3/4].count() << " ms\n";
    cout << "Centile #99 = " << samples[samples.size()*99/100].count() << " ms\n";
    cout << "Max = " << samples[samples.size()-1].count() << " ms\n";


    return 0;
}
