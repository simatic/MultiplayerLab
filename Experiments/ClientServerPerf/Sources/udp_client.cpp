//
// Application to make statistics on udp
// inspired by blocking_udp_echo_client.cpp (from https://www.boost.org/doc/libs/1_35_0/doc/html/boost_asio/example/echo/blocking_udp_echo_client.cpp)
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//

#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include "common.h"

using boost::asio::ip::udp;

using namespace std;

void analyze_packet(string_view msg_sv, unsigned char& myId)
{
    std::string msg_string{msg_sv};
    std::istringstream msg_stream{ msg_string };
    unsigned char msg_typ;
    msg_stream >> msg_typ;
    switch (ServerMsgId server_msg_typ{msg_typ }; server_msg_typ)
    {
        case ServerMsgId::IdResponse :
            struct ServerIdResponse sir;
            {
                cereal::BinaryInputArchive iarchive(msg_stream); // Create an input archive
                iarchive(sir); // Read the data from the archive
            }
            myId = sir.id;
            break;
        case ServerMsgId::BroadcastMessage :
            struct ServerBroadcastMessage sbm;
            {
                cereal::BinaryInputArchive iarchive(msg_stream); // Create an input archive
                iarchive(sbm); // Read the data from the archive
            }
            std::chrono::duration<double, std::milli> elapsed = std::chrono::system_clock::now() - sbm.sendTime;
            cout << "Received broadcast message #" << sbm.messageId << " echoed from ";
            if (sbm.senderId == myId)
                cout << "myself";
            else
                cout << "client " << static_cast<unsigned int>(sbm.senderId);
            // C++20 operator<< is not yet implemented for duration in gcc.
            // Thus, we cannot write: cout << " in " << elapsed << "\n";
            // ==> We do it manually.
            cout << " in " << elapsed.count() << " ms\n";
            break;
    }
}

[[noreturn]]void msg_receive(udp::socket &s, unsigned char &myId)
{
    for (;;)
    {
        char msg[max_length];
        udp::endpoint sender_endpoint;
        size_t msg_length = s.receive_from(
                boost::asio::buffer(msg, max_length), sender_endpoint);
        std::string_view msg_sv{msg, msg_length};
        analyze_packet(msg_sv, myId);
    }
}

int main(int argc, char* argv[])
{
    unsigned char myId{ 0 };
    try
    {
        if (argc != 3)
        {
          std::cerr << "Usage: blocking_udp_echo_client <host> <port>\n";
          return 1;
        }

        boost::asio::io_service io_service;

        udp::socket s(io_service, udp::endpoint(udp::v4(), 0));

        udp::resolver resolver(io_service);
        udp::resolver::query query(udp::v4(), argv[1], argv[2]);
        udp::resolver::iterator iterator = resolver.resolve(query);

        // Create a thread for receiving data
        auto t = jthread(msg_receive, std::ref(s), std::ref(myId));
        t.detach();

        // Send IdRequest to server
        stringstream ir_stream;
        ir_stream << static_cast<unsigned char>(ClientMsgId::IdRequest);
        std::string_view ir_sv{ ir_stream.view() };
        s.send_to(boost::asio::buffer(ir_sv.data(), ir_sv.length()), *iterator);

        // Wait for IdResponse from server (received by msg_receive thread)
        while (myId == 0)
        {
            this_thread::sleep_for(50ms);
        }

        cout << "Received my Id which is: " << static_cast<unsigned int>(myId) << "\n";

        for (unsigned int i = 0; i < 10; ++i) {
            cout << "Request to broadcast message #" << i << "\n";
            std::stringstream cmtb_stream;
            cmtb_stream << static_cast<unsigned char>(ClientMsgId::MessageToBroadcast);
            {
                cereal::BinaryOutputArchive oarchive(cmtb_stream); // Create an output archive
                struct ClientMessageToBroadcast cmtb { myId, i, std::chrono::system_clock::now() };
                oarchive(cmtb); // Write the data to the archive
            } // archive goes out of scope, ensuring all contents are flushed
            std::string_view cmtb_sv{ cmtb_stream.view() };
            s.send_to(boost::asio::buffer(cmtb_sv.data(), cmtb_sv.length()), *iterator);

            this_thread::sleep_for(500ms);
        }

        // Send DisconnectInfo to server
        stringstream di_stream;
        di_stream << static_cast<unsigned char>(ClientMsgId::DisconnectInfo);
        {
            cereal::BinaryOutputArchive oarchive(di_stream); // Create an output archive
            struct ClientDisconnectInfo cdi { myId };
            oarchive(cdi); // Write the data to the archive
        } // archive goes out of scope, ensuring all contents are flushed
        std::string_view di_sv{ di_stream.view() };
        s.send_to(boost::asio::buffer(di_sv.data(), di_sv.length()), *iterator);
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
