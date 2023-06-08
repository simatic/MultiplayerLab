//
// blocking_tcp_echo_client.cpp (from https://www.boost.org/doc/libs/1_35_0/doc/html/boost_asio/example/echo/blocking_tcp_echo_client.cpp)
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2008 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include "common.h"

using boost::asio::ip::tcp;
using namespace std;

void analyze_packet(string_view msg_sv, unsigned char& myId)
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
            std::chrono::duration<double, std::milli> elapsed = std::chrono::system_clock::now() - sbm.sendTime;
            cout << "Received broadcast message #" << sbm.messageId << " echoed from ";
            if (sbm.senderId == myId)
                cout << "myself";
            else
                cout << "client " << static_cast<unsigned int>(sbm.senderId);
            // C++20 operator<< is not yet implemented for duration in gcc.
            // Thus we cannot write: cout << " in " << elapsed << "\n";
            // ==> We do it manually.
            cout << " in " << elapsed.count() << " ms\n";
            break;
    }
}

void msg_receive(tcp::socket &s, unsigned char &myId)
{
    while (true)
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
          std::cerr << "Usage: blocking_tcp_echo_client <host> <port>\n";
          return 1;
        }

        boost::asio::io_service io_service;

        tcp::resolver resolver(io_service);
        tcp::resolver::query query(tcp::v4(), argv[1], argv[2]);
        tcp::resolver::iterator iterator = resolver.resolve(query);

        tcp::socket s(io_service);
        s.connect(*iterator);

        // Create a thread for receiving data
        auto t = jthread(msg_receive, std::ref(s), std::ref(myId));

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

        cout << "Received my Id which is: " << static_cast<unsigned int>(myId) << "\n";

        for (unsigned int i = 0; i < 10; ++i) {
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

            this_thread::sleep_for(500ms);
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
