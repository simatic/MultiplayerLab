//
// blocking_udp_echo_server.cpp (from https://www.boost.org/doc/libs/1_35_0/doc/html/boost_asio/example/echo/blocking_udp_echo_server.cpp)
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2008 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <iostream>
#include <boost/asio.hpp>
#include "common.h"

using boost::asio::ip::udp;
using namespace std;

void analyze_packet(udp::socket & sock, udp::endpoint const& sender_endpoint, string_view msg_sv, unsigned char& lastId)
{
    std::string msg_string{msg_sv};
    std::istringstream msg_stream{ msg_string };
    unsigned char msg_typ;
    msg_stream >> msg_typ;
    switch (Client client_msg_typ{ msg_typ }; client_msg_typ)
    {
        case Client::IdRequest :
        {
            std::stringstream sir_stream;
            sir_stream << static_cast<unsigned char>(Server::IdResponse);
            {
                cereal::BinaryOutputArchive oarchive(sir_stream); // Create an output archive
                struct ServerIdResponse sir { ++lastId };
                oarchive(sir); // Write the data to the archive
            } // archive goes out of scope, ensuring all contents are flushed
            std::string_view sir_sv{ sir_stream.view() };
            sock.send_to(boost::asio::buffer(sir_sv.data(), sir_sv.length()), sender_endpoint);
            break;
        }
        case Client::MessageToBroadcast :
        {
            struct ClientMessageToBroadcast cmtb;
            {
                cereal::BinaryInputArchive iarchive(msg_stream); // Create an input archive
                iarchive(cmtb); // Read the data from the archive
            }
            std::stringstream sbm_stream;
            sbm_stream << static_cast<unsigned char>(Server::BroadcastMessage);
            {
                cereal::BinaryOutputArchive oarchive(sbm_stream); // Create an output archive
                struct ServerBroadcastMessage sbm { cmtb.senderId, cmtb.messageId, cmtb.sendTime };
                oarchive(sbm); // Write the data to the archive
            } // archive goes out of scope, ensuring all contents are flushed
            std::string_view sbm_sv{ sbm_stream.view() };
            sock.send_to(boost::asio::buffer(sbm_sv.data(), sbm_sv.length()), sender_endpoint);
            break;
        }
    }
}

[[noreturn]]void server(boost::asio::io_service& io_service, short port)
{
    unsigned char lastId = 0;
    udp::socket sock(io_service, udp::endpoint(udp::v4(), port));
    for (;;)
    {
        char msg[max_length];
        udp::endpoint sender_endpoint;
        size_t msg_length = sock.receive_from(
                boost::asio::buffer(msg, max_length), sender_endpoint);
        std::string_view msg_sv{msg, msg_length};
        analyze_packet(sock, sender_endpoint, msg_sv, lastId);
    }
}

int main(int argc, char* argv[])
{
  try
  {
    if (argc != 2)
    {
      std::cerr << "Usage: blocking_udp_echo_server <port>\n";
      return 1;
    }

    boost::asio::io_service io_service;

    using namespace std; // For atoi.
    server(io_service, atoi(argv[1]));
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
