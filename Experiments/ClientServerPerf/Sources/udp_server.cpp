//
// Application to make statistics on udp
// inspired by blocking_udp_echo_server.cpp (from https://www.boost.org/doc/libs/1_35_0/doc/html/boost_asio/example/echo/blocking_udp_echo_server.cpp)
//

#include <cstdlib>
#include <iostream>
#include <map>
#include <boost/asio.hpp>
#include "common.h"

using boost::asio::ip::udp;
using namespace std;

void analyze_packet(udp::socket & sock, udp::endpoint const& sender_endpoint, string_view msg_sv, unsigned char& lastId)
{
    static map<unsigned char, udp::endpoint> mapEndPoint;
    std::string msg_string{msg_sv};
    std::istringstream msg_stream{ msg_string };
    unsigned char msg_typ;
    msg_stream >> msg_typ;
    switch (ClientMsgId client_msg_typ{msg_typ }; client_msg_typ)
    {
        case ClientMsgId::IdRequest :
        {
            std::stringstream sir_stream;
            sir_stream << static_cast<unsigned char>(ServerMsgId::IdResponse);
            {
                cereal::BinaryOutputArchive oarchive(sir_stream); // Create an output archive
                struct ServerIdResponse sir { ++lastId };
                oarchive(sir); // Write the data to the archive
            } // archive goes out of scope, ensuring all contents are flushed
            std::string_view sir_sv{ sir_stream.view() };
            sock.send_to(boost::asio::buffer(sir_sv.data(), sir_sv.length()), sender_endpoint);
            mapEndPoint[lastId] = sender_endpoint;
            break;
        }
        case ClientMsgId::MessageToBroadcast :
        {
            struct ClientMessageToBroadcast cmtb;
            {
                cereal::BinaryInputArchive iarchive(msg_stream); // Create an input archive
                iarchive(cmtb); // Read the data from the archive
            }
            std::stringstream sbm_stream;
            sbm_stream << static_cast<unsigned char>(ServerMsgId::BroadcastMessage);
            {
                cereal::BinaryOutputArchive oarchive(sbm_stream); // Create an output archive
                struct ServerBroadcastMessage sbm { cmtb.senderId, cmtb.messageId, cmtb.sendTime };
                oarchive(sbm); // Write the data to the archive
            } // archive goes out of scope, ensuring all contents are flushed
            std::string_view sbm_sv{ sbm_stream.view() };
            // We broadcast the message to all clients
            for (auto const&[id, endpoint] : mapEndPoint)
                sock.send_to(boost::asio::buffer(sbm_sv.data(), sbm_sv.length()), endpoint);
            break;
        }
        case ClientMsgId::DisconnectInfo:
        {
            struct ClientDisconnectInfo cdi;
            {
                cereal::BinaryInputArchive iarchive(msg_stream); // Create an input archive
                iarchive(cdi); // Read the data from the archive
            }
            mapEndPoint.erase(cdi.id);
            cout << "ClientMsgId disconnected\n";
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
