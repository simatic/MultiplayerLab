//
// Application to make statistics on tcp
// inspired by blocking_tcp_echo_server.cpp (from https://www.boost.org/doc/libs/1_35_0/doc/html/boost_asio/example/echo/blocking_tcp_echo_server.cpp)
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//

#include <iostream>
#include <thread>
#include <shared_mutex>
#include <boost/asio.hpp>
#include "common.h"

using boost::asio::ip::tcp;
using namespace std;

void analyze_packet(tcp::socket *psock, string_view msg_sv, unsigned char& lastId, vector<tcp::socket*> &vecSock, std::shared_timed_mutex &rw_mutex)
{
    std::string msg_string{msg_sv};
    std::istringstream msg_stream{ msg_string };
    unsigned char msg_typ;
    msg_stream >> msg_typ;
    switch (Client client_msg_typ{ msg_typ }; client_msg_typ)
    {
        case Client::DoneSendingMessages :
        {
            struct ClientDoneSendingMessages cdsm;
            {
                cereal::BinaryInputArchive iarchive(msg_stream); // Create an input archive
                iarchive(cdsm); // Read the data from the archive
            }
            // Send AckDoneSendingMessages to client
            stringstream adsm_stream;
            adsm_stream << static_cast<unsigned char>(Server::AckDoneSendingMessages);
            std::string_view adsm_sv{adsm_stream.view() };
            size_t len = adsm_sv.length();
            {
                std::lock_guard writerLock(rw_mutex);
                boost::asio::write(*psock, boost::asio::buffer(&len, sizeof(len)));
                boost::asio::write(*psock, boost::asio::buffer(adsm_sv.data(), len));
            }
            // Remove psock from vecsock
            {
                std::lock_guard writerLock(rw_mutex);
                vecSock.erase(remove(vecSock.begin(), vecSock.end(), psock), vecSock.end());
                std::erase(vecSock, psock); // C++ 20 offers this remplacement for vecSock.erase(remove())
            }
            cout << "Client " << static_cast<unsigned int>(cdsm.id) << " announces it will disconnect\n";
        }
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
            {
                std::lock_guard writerLock(rw_mutex);
                boost::asio::write(*psock, boost::asio::buffer(&len, sizeof(len)));
                boost::asio::write(*psock, boost::asio::buffer(sir_sv.data(), len));
            }
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
            // We broadcast the message to all clients
            {
                std::lock_guard writerLock(rw_mutex);
                for (auto &ps : vecSock)
                {
                    boost::asio::write(*ps, boost::asio::buffer(&len, sizeof(len)));
                    boost::asio::write(*ps, boost::asio::buffer(sbm_sv.data(), len));
                }
            }
            break;
        }
    }
}

void session(unique_ptr<tcp::socket> upsock, unsigned char& lastId)
{
    static vector<tcp::socket*> vecSock;
    static std::shared_timed_mutex rw_mutex;
    {
        std::lock_guard writerLock(rw_mutex);
        vecSock.push_back(upsock.get());
    }
    try
    {
        for (;;)
        {
            size_t len;
            boost::system::error_code error;
            size_t length = boost::asio::read(*upsock, boost::asio::buffer(&len, sizeof(len)), error);
            if (error == boost::asio::error::eof)
                break; // Connection closed cleanly by peer.
            else if (error)
                throw boost::system::system_error(error); // Some other error.
            assert(length == sizeof(len));
            char msg[max_length];
            size_t msg_length = boost::asio::read(*upsock,
                                                  boost::asio::buffer(msg, len));
            assert(msg_length == len);
            std::string_view msg_sv{msg, len};
            analyze_packet(upsock.get(), msg_sv, lastId, vecSock, rw_mutex);
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception in thread: " << e.what() << "\n";
    }
}

[[noreturn]] void server(boost::asio::io_service& io_service, short port)
{
    unsigned char lastId = 0;
    tcp::acceptor a(io_service, tcp::endpoint(tcp::v4(), port));
    for (;;)
    {
        auto upsock = make_unique<tcp::socket>(io_service);
        a.accept(*upsock);

        boost::asio::ip::tcp::no_delay option(true);
        upsock->set_option(option);

        auto t = jthread(session, std::move(upsock), ref(lastId));
        t.detach();
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

    server(io_service, atoi(argv[1]));
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
