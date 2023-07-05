//
// Application to make statistics on tcp
// inspired by blocking_tcp_echo_server.cpp (from https://www.boost.org/doc/libs/1_35_0/doc/html/boost_asio/example/echo/blocking_tcp_echo_server.cpp)
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//

#include <iostream>
#include <map>
#include <enet/enet.h>
#include "common.h"
#include "OptParserExtended.h"
#include "enet_communication.h"

using namespace std;
using namespace mlib;

struct Param {
    int port{};
    int enet_flags{};
    bool verbose{false};
};

void analyzePacket(ENetHost* server, ENetEvent const& event, Param const& param)
{
    static unsigned char lastId{0};
    static int nbBroadcastingClients{0};
    static map<unsigned char, ENetPeer*> mapEndPoint;
    std::string msgString{bit_cast<char*>(event.packet->data), event.packet->dataLength};
    switch (ClientMsgId clientMsgTyp{ static_cast<ClientMsgId>(msgString[0]) }; clientMsgTyp)
    {
        case ClientMsgId::DisconnectIntent :
        {
            auto cdi{deserializeStruct<ClientDisconnectIntent>(msgString)};
            auto s{serializeStruct<ServerAckDisconnectIntent>(ServerAckDisconnectIntent{ServerMsgId::AckDisconnectIntent})};
            sendPacket(event.peer, 0, param.enet_flags, s);
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
            sendPacket(event.peer, 0, param.enet_flags, s);
            if (param.verbose)
                cout << "Client #" << static_cast<unsigned int>(cdsm.id) << " announces it is done broadcasting messages\n";
            if (--nbBroadcastingClients == 0)
            {
                if (param.verbose)
                    cout << "There is no more broadcasting clients\n";
                auto sbe{serializeStruct<ServerBroadcastEnd>(ServerBroadcastEnd{ServerMsgId::BroadcastEnd})};
                broadcastPacket(server, 0, param.enet_flags, sbe);
            }
            break;
        }
        case ClientMsgId::IdRequest :
        {
            auto cir{deserializeStruct<ClientIdRequest>(msgString)};
            ++lastId;
            auto s{serializeStruct<ServerIdResponse>(ServerIdResponse{ServerMsgId::IdResponse, lastId})};
            sendPacket(event.peer, 0, param.enet_flags, s);
            mapEndPoint[lastId] = event.peer;
            if (param.verbose)
                cout << "Client #" << static_cast<unsigned int>(lastId) << " has received its id\n";
            if (mapEndPoint.size() == cir.nbClients)
            {
                if (param.verbose)
                    cout << "All clients are connected: They can start broadcasting\n";
                nbBroadcastingClients = cir.nbClients;
                auto sbb{serializeStruct<ServerBroadcastBegin>(ServerBroadcastBegin{ServerMsgId::BroadcastBegin})};
                broadcastPacket(server, 0, param.enet_flags, sbb);
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
            broadcastPacket(server, 0, param.enet_flags, s);
            break;
        }
        default:
        {
            cerr << "ERROR: Unexpected clientMsgTyp (" << static_cast<int>(clientMsgTyp) << ")\n";
            exit(EXIT_FAILURE);
        }

    }
}

void analyzeEvent(ENetHost* server, ENetEvent const& event, Param const& param)
{
    switch (event.type)
    {
        case ENET_EVENT_TYPE_CONNECT:
            if (param.verbose)
                cout << "A new client connected from Host "
                     << showbase << hex << event.peer->address.host << ":" << dec << event.peer->address.port << "\n";
            /* Store any relevant client information in event.peer->data. */
            break;
        case ENET_EVENT_TYPE_RECEIVE:
            analyzePacket(server, event, param);
            /* Clean up the packet now that we're done using it. */
            enet_packet_destroy(event.packet);
            break;
        case ENET_EVENT_TYPE_DISCONNECT:
            if (param.verbose)
                cout << "Host "
                     << showbase << hex << event.peer->address.host << ":" << dec << event.peer->address.port
                     << " disconnected.\n";
            /* Reset the peer's client information. */
            event.peer->data = nullptr;
        case ENET_EVENT_TYPE_NONE: // Happens when no event occurred within the specified time limit ==> We just ignore this event.
            break;
    }

}

[[noreturn]] void server(Param const& param)
{
    if (enet_initialize() != 0)
    {
        cerr << "ERROR: An error occurred while initializing ENet.\n";
        exit(EXIT_FAILURE);
    }
    atexit(enet_deinitialize);

    ENetAddress address{ ENET_HOST_ANY, static_cast<enet_uint16>(param.port) }; // ENET_HOST_ANY means that anyone can connect to the server

    ENetHost* server = enet_host_create(&address, 128, 1, 0, 0); // 128 = number of clients allowed to connect, 1 = number of channels
    if (server == nullptr)
    {
        cerr << "ERROR: An error occurred while trying to create ENet server host (error could be because there is already a running server bound to port " << param.port << ").\n";
        exit(EXIT_FAILURE);
    }

    for (;;)
    {
        ENetEvent event;
        while (enet_host_service(server, &event, timeoutFor_enet_host_service) > 0)
            analyzeEvent(server, event, param);
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
            "u|unreliable \t [optional] By default, ENet will apply ENET_PACKET_FLAG_RELIABLE flag to packets sent.\n\t\t\t\t\tIf this flag is specified, ENET_PACKET_FLAG_UNSEQUENCED flag will be applied\n\t\t\t\t\t(packets will not be sequenced with other packet and will not be reliably sent)",
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
            parser.hasopt ('v') ? ENET_PACKET_FLAG_UNSEQUENCED : ENET_PACKET_FLAG_RELIABLE,
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
