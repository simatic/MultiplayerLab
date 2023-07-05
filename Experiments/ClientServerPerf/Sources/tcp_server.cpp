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
#include "OptParserExtended.h"
#include "tcp_communication.h"

using boost::asio::ip::tcp;
using namespace std;
using namespace mlib;

struct Param {
    int port;
    bool verbose{false};
};

void broadcastMsg(std::string const& s, map<unsigned char, tcp::socket*> const& mapEndPoint)
{
    for (auto const&[id, endpoint] : mapEndPoint)
    {
        sendPacket(endpoint, s);
    }

}

void analyzePacket(tcp::socket *psock, const string &msgString, Param const& param)
{
    static atomic_uchar lastId{0};
    static atomic_int32_t nbBroadcastingClients{0};
    static map<unsigned char, tcp::socket*> mapEndPoint;
    static std::shared_timed_mutex rwMutex;
    switch (ClientMsgId clientMsgTyp{ static_cast<ClientMsgId>(msgString[0]) }; clientMsgTyp)
    {
        case ClientMsgId::DisconnectIntent :
        {
            auto cdi{deserializeStruct<ClientDisconnectIntent>(msgString)};
            auto s{serializeStruct<ServerAckDisconnectIntent>(ServerAckDisconnectIntent{ServerMsgId::AckDisconnectIntent})};
            sendPacket(psock, s);
            // Remove psock from vecsock
            {
                std::lock_guard writerLock(rwMutex);
                mapEndPoint.erase(cdi.id);
            }
            if (param.verbose)
                cout << "Client #" << static_cast<unsigned int>(cdi.id) << " announces it will disconnect\n";
            break;
        }
        case ClientMsgId::DoneSendingMessages :
        {
            auto cdsm{deserializeStruct<ClientDoneSendingMessages>(msgString)};
            auto s{serializeStruct<ServerAckDoneSendingMessages>(ServerAckDoneSendingMessages{ServerMsgId::AckDoneSendingMessages})};
            sendPacket(psock, s);
            if (param.verbose)
                cout << "Client #" << static_cast<unsigned int>(cdsm.id) << " announces it is done broadcasting messages\n";
            if (--nbBroadcastingClients == 0)
            {
                if (param.verbose)
                    cout << "There is no more broadcasting clients\n";
                auto sbe{serializeStruct<ServerBroadcastEnd>(ServerBroadcastEnd{ServerMsgId::BroadcastEnd})};
                {
                    std::shared_lock readerLock(rwMutex);
                    broadcastMsg(sbe, mapEndPoint);
                }
            }
            break;
        }
        case ClientMsgId::IdRequest :
        {
            auto cir{deserializeStruct<ClientIdRequest>(msgString)};
            unsigned char idToReturn = ++lastId;
            auto s{serializeStruct<ServerIdResponse>(ServerIdResponse{ServerMsgId::IdResponse, idToReturn})};
            sendPacket(psock, s);
            {
                std::lock_guard writerLock(rwMutex);
                mapEndPoint[idToReturn] = psock;
                if (param.verbose)
                    cout << "Client #" << static_cast<unsigned int>(idToReturn) << " has received its id\n";
                if (mapEndPoint.size() == cir.nbClients)
                {
                    if (param.verbose)
                        cout << "All clients are connected: They can start broadcasting\n";
                    nbBroadcastingClients = cir.nbClients;
                    auto sbb{serializeStruct<ServerBroadcastBegin>(ServerBroadcastBegin{ServerMsgId::BroadcastBegin})};
                    broadcastMsg(sbb, mapEndPoint);
                }
            }
            break;
        }
        case ClientMsgId::MessageToBroadcast :
        {
            auto cmtb{deserializeStruct<ClientMessageToBroadcast>(msgString)};
            auto s {serializeStruct<ServerBroadcastMessage>(ServerBroadcastMessage{ServerMsgId::BroadcastMessage,
                                                                                   cmtb.senderId,
                                                                                   cmtb.msgNum,
                                                                                   cmtb.sendTime, cmtb.filler})};
            {
                std::shared_lock readerLock(rwMutex);
                broadcastMsg(s, mapEndPoint);
            }
            break;
        }
        default:
        {
            cerr << "ERROR : Unexpected clientMsgTyp (" << static_cast<int>(clientMsgTyp) << ")\n";
            exit(EXIT_FAILURE);
        }

    }
}

void session(unique_ptr<tcp::socket> upsock, Param const& param)
{
    try
    {
        for (;;)
        {
            auto s{receivePacket(upsock.get())};
            analyzePacket(upsock.get(), s, param);
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

[[noreturn]] void server(boost::asio::io_service& ioService, short port, Param const& param)
{
    tcp::acceptor a(ioService, tcp::endpoint(tcp::v4(), port));
    for (;;)
    {
        auto upsock = make_unique<tcp::socket>(ioService);
        a.accept(*upsock);

        boost::asio::ip::tcp::no_delay option(true);
        upsock->set_option(option);

        auto t = jthread(session, std::move(upsock), ref(param));
        t.detach();
    }
}

struct Param getParam(int argc, char* argv[])
{
    OptParserExtended parser{
            "h|help \t Show help message",
            "p:port port_number \t Port to connect to",
            "v|verbose \t [optional] Verbose display required"
    };

    int nonopt;
    if (int ret ; (ret = parser.parse (argc, argv, &nonopt)) != 0)
    {
        if (ret == 1)
            cerr << "Unknown option: " << argv[nonopt] << " Valid options are : " << endl
                 << parser.synopsis () << endl;
        else if (ret == 2)
            cerr << "Option " << argv[nonopt] << " requires an argument." << endl;
        else if (ret == 3)
            cerr << "Invalid options combination: " << argv[nonopt] << endl;
        exit (1);
    }
    if ((argc == 1) || parser.hasopt ('h'))
    {
        //No arguments on command line or help required. Show help and exit.
        cerr << "Usage:" << endl;
        cerr << parser.synopsis () << endl;
        cerr << "Where:" << endl
             << parser.description () << endl;
        exit (0);
    }

    struct Param param{
            parser.getoptIntRequired('p'),
            parser.hasopt ('v')
    };

    if (nonopt < argc)
    {
        cerr << "ERROR: There is a non-option argument '" << argv[nonopt]
             << "' which cannot be understood. Please run again program but without this argument" << endl
             << parser.synopsis () << endl;
        exit(1);
    }

    return param;
}

int main(int argc, char* argv[])
{
    struct Param param{getParam(argc, argv)};
    try
    {
        boost::asio::io_service ioService;
        server(ioService, static_cast<short>(param.port), param);
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
