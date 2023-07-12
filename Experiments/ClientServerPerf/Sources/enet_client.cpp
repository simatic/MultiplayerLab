//
// Application to make statistics on tcp
// inspired by blocking_tcp_echo_client.cpp (from https://www.boost.org/doc/libs/1_35_0/doc/html/boost_asio/example/echo/blocking_tcp_echo_client.cpp)
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//

#include <enet/enet.h>
#include <iostream>
#include <latch>
#include <thread>
#include <vector>
#include "common.h"
#include "enet_communication.h"
#include "Measures.h"
#include "options.h"
#include "OptParserExtended.h"

using namespace std;
using namespace mlib;

struct Param {
    string host;
    int port;
    int nbMsg{0};
    int sizeMsg{0};
    int nbClients{0};
    int enet_flags{};
    bool verbose{false};

    static std::string csvHeadline()
    {
        return std::string { "host,port,nbMsg,sizeMsg,nbClients"};
    }

    [[nodiscard]] std::string asCsv() const
    {
        return std::string { host + "," + std::to_string(port) + "," + std::to_string(nbMsg) + "," + std::to_string(sizeMsg) + "," + std::to_string(nbClients) };
    }
};

unsigned int sendMsgToBroadcast(ENetPeer* peer, Param const &param, unsigned char myId, unsigned int msgNum)
{
    ++msgNum;
    if (param.verbose)
        cout << "Client #" << static_cast<unsigned int>(myId)  << " : Request to broadcast message #" << msgNum << "\n";
    auto s_cmtb {serializeStruct<ClientMessageToBroadcast>(ClientMessageToBroadcast{ClientMsgId::MessageToBroadcast,
                                                                                    myId, msgNum,
                                                                                    std::chrono::system_clock::now(),
                                                                                    std::string(
                                                                                            param.sizeMsg -
                                                                                            minSizeClientMessageToBroadcast,
                                                                                            0)})};
    sendPacket(peer, 0, param.enet_flags, s_cmtb);
    return msgNum;
}

// Should return true if handlePacket discovers client will not receive any more packets.
bool handlePacket(ENetEvent const &event, Param const &param, Measures &measures, std::latch &allAckDisconnectIntentReceived)
{
    static thread_local unsigned char myId{0};
    static thread_local unsigned int msgNum{0};
    std::string msgString{bit_cast<char*>(event.packet->data), event.packet->dataLength};
    switch (ServerMsgId serverMsgId{ static_cast<ServerMsgId>(msgString[0]) }; serverMsgId)
    {
        case ServerMsgId::AckDisconnectIntent :
            allAckDisconnectIntentReceived.count_down();
            return true;
        case ServerMsgId::AckDoneSendingMessages :
            return false;
        case ServerMsgId::BroadcastBegin :
        {
            msgNum = sendMsgToBroadcast(event.peer, param, myId, msgNum);
            return false;
        }
        case ServerMsgId::BroadcastMessage :
        {
            auto sbm {deserializeStruct<ServerBroadcastMessage>(msgString)};
            chrono::duration<double, std::milli> elapsed = std::chrono::system_clock::now() - sbm.sendTime;
            if (sbm.senderId == myId)
            {
                measures.add(elapsed);
            }
            if (param.verbose) {
                cout << "Client #" << static_cast<unsigned int>(myId) << " : ";
                cout << "Received broadcast message #" << sbm.msgNum << " echoed from ";
                if (sbm.senderId == myId)
                    cout << "myself";
                else
                    cout << "client #" << static_cast<unsigned int>(sbm.senderId);
                // C++20 operator<< is not yet implemented for duration in gcc.
                // Thus, we cannot write: " in " << elapsed << "\n";
                // ==> We do it manually.
                cout << " in " << elapsed.count() << " ms\n";
            }
            if (sbm.senderId == myId)
            {
                if (msgNum < param.nbMsg)
                {
                    msgNum = sendMsgToBroadcast(event.peer, param, myId, msgNum);
                }
                else
                {
                    auto cdsm {serializeStruct<ClientDoneSendingMessages>(ClientDoneSendingMessages{ClientMsgId::DoneSendingMessages, myId})};
                    sendPacket(event.peer, 0, param.enet_flags, cdsm);
                }
            }
            return false;
        }
        case ServerMsgId::BroadcastEnd :
        {
            auto cdi {serializeStruct<ClientDisconnectIntent>(ClientDisconnectIntent{ClientMsgId::DisconnectIntent, myId})};
            sendPacket(event.peer, 0, param.enet_flags, cdi);
            return false;
        }
        case ServerMsgId::IdResponse :
        {
            auto sir {deserializeStruct<ServerIdResponse>(msgString)};
            myId = sir.id;
            if (param.verbose)
                cout << "Client received its Id which is: #" << static_cast<unsigned int>(myId) << "\n";
            return false;
        }
        default:
        {
            cerr << "ERROR: Unexpected serverMsgId (" << static_cast<int>(serverMsgId) << ")\n";
            exit(EXIT_FAILURE);
        }
    }
}

// Returns true if there are no more events to handle
bool handleEvent(ENetEvent const &event, Param const& param, Measures & measures, std::latch &allAckDisconnectIntentReceived)
{
    switch (event.type)
    {
        case ENET_EVENT_TYPE_RECEIVE:
            handlePacket(event, param, measures, allAckDisconnectIntentReceived);
            /* Clean up the packet now that we're done using it. */
            enet_packet_destroy(event.packet);
            break;
        case ENET_EVENT_TYPE_DISCONNECT:
            // This event can only be received when we call enet_peer_disconnect()
            if (param.verbose)
                cout << "Disconnection success.\n";
            return true;
        case ENET_EVENT_TYPE_CONNECT:
            // We should never see this event as we are already connected.
            cerr << "Received unexpected ENET_EVENT_TYPE_CONNECT\n";
            exit(EXIT_FAILURE);
        case ENET_EVENT_TYPE_NONE: // Happens when no event occurred within the specified time limit ==> We just ignore this event.
            break;
    }
    return false;
}

void msgReceive(ENetHost* client, Param const& param, Measures & measures, std::latch &allAckDisconnectIntentReceived)
{
    for (;;)
    {
        ENetEvent event;
        while (enet_host_service(client, &event, timeoutFor_enet_host_service) > 0)
            if (handleEvent(event, param, measures, allAckDisconnectIntentReceived))
                return;
    }
}

void client(Param const& param, Measures & measures, std::latch &allAckDisconnectIntentReceived)
{
    if (enet_initialize() != 0)
    {
        cerr << "ERROR: An error occurred while initializing ENet.\n";
        exit(EXIT_FAILURE);
    }
    atexit(enet_deinitialize);

    ENetHost* clientHost;
    clientHost = enet_host_create(nullptr, 1, 1, 0, 0); // First '1' = Only one outgoing connection, Second '1' = number of channels
    if (clientHost == nullptr)
    {
        cerr << "ERROR: An error occurred while creating ENet client host.\n";
        exit(EXIT_FAILURE);
    }

    ENetAddress address; // Address of the server
    ENetPeer* peer;  // Contact point to the server
    enet_address_set_host(&address, param.host.c_str());
    address.port = static_cast<enet_uint16>(param.port);
    peer = enet_host_connect(clientHost, &address, 1, 0); // Connection with no data sent during connection
    if (peer == nullptr)
    {
        cerr << "ERROR: No available peers for initiating an ENet connection.\n";
        exit(EXIT_FAILURE);
    }
    if (ENetEvent event ; enet_host_service(clientHost, &event, 2000) > 0 && // To check that the server contacted us back. 2000 = Nb milliseconds we want to wait.
                          event.type == ENET_EVENT_TYPE_CONNECT)
    {
        if (param.verbose)
            cout << "Connection to server at " << param.host << ":" << param.port << " succeeded.\n";
    }
    else
    {
        enet_peer_reset(peer);
        cerr << "Connection to server at " << param.host << ":" << param.port << " FAILED (This could be because the server is not running).\n";
        exit(EXIT_FAILURE);
    }

    // Create a thread for receiving data
    auto t = jthread(msgReceive, clientHost, std::ref(param), std::ref(measures), std::ref(allAckDisconnectIntentReceived));

    // Send IdRequest to server
    auto cir {serializeStruct<ClientIdRequest>(ClientIdRequest{ClientMsgId::IdRequest, param.nbClients})};
    sendPacket(peer, 0, param.enet_flags, cir);

    allAckDisconnectIntentReceived.wait();

    enet_peer_disconnect(peer, 0); // Instead of 0, we could have sent some data

    // msgReceive thread will exit when it has received ENET_EVENT_TYPE_DISCONNECT
    t.join();

    if (param.verbose)
        cout << "Client done\n";
}

int main(int argc, char* argv[])
{
    //
    // Take care of program arguments
    //
    OptParserExtended parser{
            "h|help \t Show help message",
            "H:host hostname \t Host (or IP address) to connect to",
            "p:port port_number \t Port to connect to",
            "n:nbMsg number \t Number of messages to be sent",
            "s:size size_in_bytes \t Size of messages sent by a client (must be in interval [22,65515])",
            "c:clients number \t Number of clients which send messages to server",
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
            parser.getoptStringRequired('H'),
            parser.getoptIntRequired('p'),
            parser.getoptIntRequired('n'),
            parser.getoptIntRequired('s'),
            parser.getoptIntRequired('c'),
            parser.hasopt ('v') ? ENET_PACKET_FLAG_UNSEQUENCED : ENET_PACKET_FLAG_RELIABLE,
            parser.hasopt ('v')
    };
    if (param.sizeMsg < minSizeClientMessageToBroadcast || param.sizeMsg > maxLength)
    {
        cerr << "ERROR: Argument for size of messages is " << param.sizeMsg
             << " which is not in interval [ " << minSizeClientMessageToBroadcast << " , " << maxLength << " ]"
             << endl
             << parser.synopsis () << endl;
        exit(1);
    }

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

    // Variables used during experiment
    Measures measures(param.nbMsg * param.nbClients);
    std::latch allAckDisconnectIntentReceived(param.nbClients);

    // We launch all the clients to make the experiment
    vector<jthread> clients(param.nbClients);
    for (auto& c : clients) {
        c = jthread(client, std::ref(param), std::ref(measures), std::ref(allAckDisconnectIntentReceived));
    }
    for (auto& c: clients)
        c.join();

    // Display statistics
    cout << "Protocol," << Param::csvHeadline() << "," << Measures::csvHeadline() << "\n";
    cout << (param.enet_flags == ENET_PACKET_FLAG_RELIABLE ? "enet reliable," : "enet UNreliable,") << param.asCsv() << "," << measures.asCsv(param.nbMsg, param.nbClients) << "\n";

    return 0;
}
