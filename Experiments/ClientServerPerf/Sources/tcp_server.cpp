//
// Application to make statistics on tcp
// inspired by blocking_tcp_echo_server.cpp (from https://www.boost.org/doc/libs/1_35_0/doc/html/boost_asio/example/echo/blocking_tcp_echo_server.cpp)
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//

#include <iostream>
#include <map>
#include <thread>
#include <shared_mutex>
#include <boost/asio.hpp>
#include "common.h"
#include "OptParser_ext.h"
#include "tcp_communication.h"

using boost::asio::ip::tcp;
using namespace std;
using namespace mlib;

struct param_t {
    int port;
    bool verbose{false};
};

void analyze_packet(tcp::socket *psock, string_view msg_sv, param_t const& param, atomic_uchar& lastId)
{
    static map<unsigned char, tcp::socket*> mapEndPoint;
    static std::shared_timed_mutex rw_mutex;
    std::string msg_string{msg_sv};
    std::istringstream msg_stream{ msg_string };
    unsigned char msg_id;
    msg_stream >> msg_id;
    switch (ClientMsgId client_msg_typ{msg_id }; client_msg_typ)
    {
        case ClientMsgId::DoneSendingMessages :
        {
            auto cdsm{read_data_in_msg_stream<ClientDoneSendingMessages>(msg_stream)};
            auto s{prepare_msg_with_no_data<ServerMsgId>(ServerMsgId::AckDoneSendingMessages)};
            tcp_send(psock, s);
            // Remove psock from vecsock
            {
                std::lock_guard writerLock(rw_mutex);
                mapEndPoint.erase(cdsm.id);
            }
            if (param.verbose)
                cout << "Client #" << static_cast<unsigned int>(cdsm.id) << " announces it will disconnect\n";
            break;
        }
        case ClientMsgId::IdRequest :
        {
            unsigned char idToReturn = ++lastId;
            auto s{prepare_msg<ServerMsgId, ServerIdResponse>(ServerMsgId::IdResponse,
                                                                ServerIdResponse{idToReturn })};
            tcp_send(psock, s);
            {
                std::lock_guard writerLock(rw_mutex);
                mapEndPoint[idToReturn] = psock;
            }
            break;
        }
        case ClientMsgId::MessageToBroadcast :
        {
            auto cmtb{read_data_in_msg_stream<ClientMessageToBroadcast>(msg_stream)};
            auto s {prepare_msg<ServerMsgId, ServerBroadcastMessage>(ServerMsgId::BroadcastMessage,
                                                                      ServerBroadcastMessage{ cmtb.senderId, cmtb.messageId, cmtb.sendTime, cmtb.filler })};
            // We broadcast the message to all clients
            {
                std::shared_lock readerLock(rw_mutex);
                for (auto const&[id, endpoint] : mapEndPoint)
                {
                    tcp_send(endpoint, s);
                }
            }
            break;
        }
        default:
        {
            cerr << "ERROR : Unexpected client_msg_typ (" << static_cast<int>(client_msg_typ) << ")\n";
            exit(EXIT_FAILURE);
        }

    }
}

void session(unique_ptr<tcp::socket> upsock, param_t const& param, atomic_uchar& lastId)
{
    try
    {
        for (;;)
        {
            auto msg_s{tcp_receive(upsock.get())};
            analyze_packet(upsock.get(), msg_s, param, lastId);
        }
    }
    catch (boost::system::system_error& e)
    {
        if (e.code() == boost::asio::error::eof)
        {
            if (param.verbose)
                cout << "Client disconnected\n";
        }
        else
            std::cerr << "Unexpected Boost Exception in thread: " << e.what() << "\n";
    }
    catch (std::exception& e)
    {
        std::cerr << "Unexpected non-Boost Exception in thread: " << e.what() << "\n";
    }
}

[[noreturn]] void server(boost::asio::io_service& io_service, short port, param_t const& param)
{
    atomic_uchar lastId{0};
    tcp::acceptor a(io_service, tcp::endpoint(tcp::v4(), port));
    for (;;)
    {
        auto upsock = make_unique<tcp::socket>(io_service);
        a.accept(*upsock);

        boost::asio::ip::tcp::no_delay option(true);
        upsock->set_option(option);

        auto t = jthread(session, std::move(upsock), ref(param), ref(lastId));
        t.detach();
    }
}

struct param_t getParam(int argc, char* argv[])
{
    OptParser_ext parser{
            "h|help \t Show help message",
            "p:port port_number \t Port to connect to",
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
            parser.getopt_required_int('p'),
            parser.hasopt ('v')
    };

    if (nonopt < argc)
        cout << "WARNING: There is a non-option argument: " << argv[nonopt] << " ==> It won't be used" << endl;

    return param;
}

int main(int argc, char* argv[])
{
    struct param_t param{getParam(argc, argv)};
    try
    {
        boost::asio::io_service io_service;
        server(io_service, static_cast<short>(param.port), param);
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
