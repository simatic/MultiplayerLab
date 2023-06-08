//
// blocking_tcp_echo_server.cpp (from https://www.boost.org/doc/libs/1_35_0/doc/html/boost_asio/example/echo/blocking_tcp_echo_server.cpp)
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2008 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <iostream>
#include <thread>
#include <boost/smart_ptr.hpp>
#include <boost/asio.hpp>
#include "common.h"

using boost::asio::ip::tcp;
using namespace std;

using socket_ptr = boost::shared_ptr<tcp::socket> ;

void analyze_packet(socket_ptr const& sock, string_view msg_sv, unsigned char& lastId)
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
            size_t len = sir_sv.length();
            boost::asio::write(*sock, boost::asio::buffer(&len, sizeof(len)));
            boost::asio::write(*sock, boost::asio::buffer(sir_sv.data(), len));
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
            size_t len = sbm_sv.length();
            boost::asio::write(*sock, boost::asio::buffer(&len, sizeof(len)));
            boost::asio::write(*sock, boost::asio::buffer(sbm_sv.data(), len));
            break;
        }
    }
}

void session(socket_ptr const& sock, unsigned char& lastId)
{
    try
    {
        for (;;)
        {
            size_t len;
            boost::system::error_code error;
            size_t length = boost::asio::read(*sock,boost::asio::buffer(&len, sizeof(len)), error);
            if (error == boost::asio::error::eof)
                break; // Connection closed cleanly by peer.
            else if (error)
                throw boost::system::system_error(error); // Some other error.
            assert(length == sizeof(len));
            char msg[max_length];
            size_t msg_length = boost::asio::read(*sock,
                                                  boost::asio::buffer(msg, len));
            assert(msg_length == len);
            std::string_view msg_sv{msg, len};
            analyze_packet(sock, msg_sv, lastId);
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception in thread: " << e.what() << "\n";
    }
    cout << "Client disconnected\n";
}

[[noreturn]] void server(boost::asio::io_service& io_service, short port)
{
    unsigned char lastId = 0;
    tcp::acceptor a(io_service, tcp::endpoint(tcp::v4(), port));
    for (;;)
    {
        socket_ptr sock(new tcp::socket(io_service));
        a.accept(*sock);
        auto t = jthread(session, sock, ref(lastId));
    }
}

int main(int argc, char* argv[])
{
  try
  {
    if (argc != 2)
    {
      std::cerr << "Usage: blocking_tcp_echo_server <port>\n";
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
