//
// Application to make statistics on tcp
// inspired by blocking_tcp_echo_server.cpp (from https://www.boost.org/doc/libs/1_35_0/doc/html/boost_asio/example/echo/blocking_tcp_echo_server.cpp)
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//

#include <iostream>
#include <map>
#include <boost/asio.hpp>
#include "common.h"
#include "OptParserExtended.h"
#include "udp_communication.h"

using boost::asio::ip::udp;
using namespace std;
using namespace mlib;

struct Param {
    int port{};
    bool verbose{false};
};

void broadcastMsg(udp::socket & sock, std::string const& s, map<unsigned char, udp::endpoint> const& mapEndPoint)
{
    for (auto const&[id, endpoint] : mapEndPoint)
        sendPacket(sock, s, endpoint);
}

void analyzePacket(udp::socket &sock, udp::endpoint const& senderEndpoint, const string &msgString, Param const& param)
{
    static unsigned char lastId{0};
    static int nbBroadcastingClients{0};
    static map<unsigned char, udp::endpoint> mapEndPoint;
    switch (ClientMsgId clientMsgTyp{ static_cast<ClientMsgId>(msgString[0]) }; clientMsgTyp)
    {
        case ClientMsgId::DisconnectIntent :
        {
            auto cdi{deserializeStruct<ClientDisconnectIntent>(msgString)};
            auto s{serializeStruct<ServerAckDisconnectIntent>(ServerAckDisconnectIntent{ServerMsgId::AckDisconnectIntent})};
            sendPacket(sock, s, senderEndpoint);
            // Remove senderEndpoint from mapEndPoint
            mapEndPoint.erase(cdi.id);
            if (param.verbose)
                cout << "Client #" << static_cast<unsigned int>(cdi.id) << " announces it will disconnect\n";
            break;
        }
        case ClientMsgId::DoneSendingMessages :
        {
            auto cdsm{deserializeStruct<ClientDoneSendingMessages>(msgString)};
            auto s{serializeStruct<ServerAckDoneSendingMessages>(ServerAckDoneSendingMessages{ServerMsgId::AckDoneSendingMessages})};
            sendPacket(sock, s, senderEndpoint);
            if (param.verbose)
                cout << "Client #" << static_cast<unsigned int>(cdsm.id) << " announces it is done broadcasting messages\n";
            if (--nbBroadcastingClients == 0)
            {
                if (param.verbose)
                    cout << "There is no more broadcasting clients\n";
                auto sbe{serializeStruct<ServerBroadcastEnd>(ServerBroadcastEnd{ServerMsgId::BroadcastEnd})};
                broadcastMsg(sock, sbe, mapEndPoint);
            }
            break;
        }
        case ClientMsgId::IdRequest :
        {
            auto cir{deserializeStruct<ClientIdRequest>(msgString)};
            ++lastId;
            auto s{serializeStruct<ServerIdResponse>(ServerIdResponse{ServerMsgId::IdResponse, lastId})};
            sendPacket(sock, s, senderEndpoint);
            mapEndPoint[lastId] = senderEndpoint;
            if (param.verbose)
                cout << "Client #" << static_cast<unsigned int>(lastId) << " has received its id\n";
            if (mapEndPoint.size() == cir.nbClients)
            {
                if (param.verbose)
                    cout << "All clients are connected: They can start broadcasting\n";
                nbBroadcastingClients = cir.nbClients;
                auto sbb{serializeStruct<ServerBroadcastBegin>(ServerBroadcastBegin{ServerMsgId::BroadcastBegin})};
                broadcastMsg(sock, sbb, mapEndPoint);
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
            broadcastMsg(sock, s, mapEndPoint);
            break;
        }
        default:
        {
            cerr << "ERROR: Unexpected clientMsgTyp (" << static_cast<int>(clientMsgTyp) << ")\n";
            exit(EXIT_FAILURE);
        }

    }
}

[[noreturn]] void server(Param const& param)
{
    try
    {
        boost::asio::io_service ioService;
        udp::socket sock(ioService, udp::endpoint(udp::v4(), static_cast<short>(param.port)));
        for (;;)
        {
            udp::endpoint senderEndpoint;
            auto msgString{receivePacket(sock, senderEndpoint)};
            analyzePacket(sock, senderEndpoint, msgString, param);
        }
    }
    catch (boost::system::system_error& e)
    {
        if (e.code() == boost::asio::error::address_in_use)
            cerr << "ERROR: Server cannot bind to port " << param.port << " (probably because there is an other server running and already bound to this port)\n";
        else
            cerr << "ERROR: Unexpected Boost Exception in function server(): " << e.what() << "\n";
        exit(1);
    }
}

int main(int argc, char* argv[])
{
    //
    // Take care of program arguments
    //
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

    //
    // Launch the application
    //
    server(param);
}
