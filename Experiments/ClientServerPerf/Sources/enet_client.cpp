//
// Application to make statistics on tcp
// inspired by blocking_tcp_echo_client.cpp (from https://www.boost.org/doc/libs/1_35_0/doc/html/boost_asio/example/echo/blocking_tcp_echo_client.cpp)
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//

#include <iostream>
#include <thread>
#include <vector>
#include <enet/enet.h>
#include <numeric>
#include <latch>
#include <algorithm>
#include "common.h"
#include "options.h"
#include "OptParserExtended.h"
#include "enet_communication.h"

using namespace std;
using namespace mlib;

struct Measures {
    explicit Measures(size_t nb_rtts_max)
    : rtts(nb_rtts_max)
    {
    }
    vector<std::chrono::duration<double, std::milli>> rtts; // Round-Trip Time
    atomic_size_t nbRtts{0};
};

struct Param {
    string host;
    int port;
    int nbMsg{0};
    chrono::duration<int64_t, ratio<1, 1000>> sendInterval{0};
    int sizeMsg{0};
    int nbClients{0};
    int enet_flags{};
    bool verbose{false};
};

// Should return true if analyzePacket discovers client will not receive any more packets.
bool analyzePacket(ENetEvent const& event, unsigned char& myId, Param const& param, Measures & measures, std::latch &broadcastBegin, std::latch &broadcastEnd, std::latch &ackDisconnectIntent)
{
    std::string msgString{bit_cast<char*>(event.packet->data), event.packet->dataLength};
    switch (ServerMsgId serverMsgId{ static_cast<ServerMsgId>(msgString[0]) }; serverMsgId)
    {
        case ServerMsgId::AckDisconnectIntent :
            ackDisconnectIntent.count_down();
            return true;
        case ServerMsgId::AckDoneSendingMessages :
            return false;
        case ServerMsgId::BroadcastBegin :
            broadcastBegin.count_down();
            return false;
        case ServerMsgId::BroadcastMessage :
        {
            auto sbm {deserializeStruct<ServerBroadcastMessage>(msgString)};
            chrono::duration<double, std::milli> elapsed = std::chrono::system_clock::now() - sbm.sendTime;
            if (sbm.senderId == myId)
            {
                measures.rtts[measures.nbRtts++] = elapsed;
            }
            if (param.verbose) {
                cout << "Client #" << static_cast<unsigned int>(myId) << " : ";
                cout << "Received broadcast message #" << sbm.msgNum << " echoed from ";
                if (sbm.senderId == myId)
                    cout << "myself";
                else
                    cout << "client " << static_cast<unsigned int>(sbm.senderId);
                // C++20 operator<< is not yet implemented for duration in gcc.
                // Thus, we cannot write: " in " << elapsed << "\n";
                // ==> We do it manually.
                cout << " in " << elapsed.count() << " ms\n";
            }
            return false;
        }
        case ServerMsgId::BroadcastEnd :
            broadcastEnd.count_down();
            return false;
        case ServerMsgId::IdResponse :
        {
            auto sir {deserializeStruct<ServerIdResponse>(msgString)};
            myId = sir.id;
            return false;
        }
        default:
        {
            cerr << "ERROR: Unexpected serverMsgId (" << static_cast<int>(serverMsgId) << ")\n";
            exit(EXIT_FAILURE);
        }
    }
}

void msgReceive(ENetHost* client, unsigned char &myId, Param const& param, Measures & measures, std::latch &broadcastBegin, std::latch &broadcastEnd, std::latch &ackDisconnectIntent)
{
    for (;;)
    {
        ENetEvent event;
        while (enet_host_service(client, &event, timeoutFor_enet_host_service) > 0)
        {
            switch (event.type)
            {
                case ENET_EVENT_TYPE_RECEIVE:
                    analyzePacket(event, myId, param, measures, broadcastBegin, broadcastEnd, ackDisconnectIntent);
                    /* Clean up the packet now that we're done using it. */
                    enet_packet_destroy(event.packet);
                    break;
                case ENET_EVENT_TYPE_DISCONNECT:
                    // This event can only be received when we call enet_peer_disconnect()
                    if (param.verbose)
                        cout << "Disconnection success.\n";
                    return;
                case ENET_EVENT_TYPE_CONNECT:
                    // We should never see this event as we are already connected.
                    cerr << "Received unexpected ENET_EVENT_TYPE_CONNECT\n";
                    exit(EXIT_FAILURE);
                case ENET_EVENT_TYPE_NONE: // Happens when no event occurred within the specified time limit ==> We just ignore this event.
                    break;
            }
        }
    }
}

void client(Param const& param, Measures & measures, std::latch &broadcastBegin, std::latch &broadcastEnd, std::latch &ackDisconnectIntent)
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

    unsigned char myId{ 0 };

    // Create a thread for receiving data
    auto t = jthread(msgReceive, clientHost, std::ref(myId), std::ref(param), std::ref(measures),
                     std::ref(broadcastBegin), std::ref(broadcastEnd), std::ref(ackDisconnectIntent));

    // Send IdRequest to server
    auto cir {serializeStruct<ClientIdRequest>(ClientIdRequest{ClientMsgId::IdRequest, param.nbClients})};
    sendPacket(peer, 0, param.enet_flags, cir);

    broadcastBegin.wait();

    if (param.verbose)
        cout << "My Id which is: " << static_cast<unsigned int>(myId) << " and I can start broadcasting\n";

    for (unsigned int i = 0; i < param.nbMsg; ++i) {
        if (param.verbose)
            cout << "Request to broadcast message #" << i << "\n";
        auto s_cmtb {serializeStruct<ClientMessageToBroadcast>(ClientMessageToBroadcast{ClientMsgId::MessageToBroadcast,
                                                                                        myId, i,
                                                                                        std::chrono::system_clock::now(),
                                                                                        std::string(
                                                                                                param.sizeMsg -
                                                                                                minSizeClientMessageToBroadcast,
                                                                                                0)})};
        sendPacket(peer, 0, param.enet_flags, s_cmtb);
        enet_host_flush(clientHost);
        this_thread::sleep_for(param.sendInterval);
    }

    // Send DoneSendingMessages to server
    auto cdsm {serializeStruct<ClientDoneSendingMessages>(ClientDoneSendingMessages{ClientMsgId::DoneSendingMessages, myId})};
    sendPacket(peer, 0, param.enet_flags, cdsm);

    broadcastEnd.wait();

    // Send DisconnectIntent to server
    auto cdi {serializeStruct<ClientDisconnectIntent>(ClientDisconnectIntent{ClientMsgId::DisconnectIntent, myId})};
    sendPacket(peer, 0, param.enet_flags, cdi);

    ackDisconnectIntent.wait();

    enet_peer_disconnect(peer, 0); // Instead of 0, we could have sent some data

    // msgReceive thread will exit when it has received ENET_EVENT_TYPE_DISCONNECT
    t.join();

    if (param.verbose)
        cout << "Client #" << static_cast<unsigned int>(myId) << " done\n";
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
            "i:interval time_in_milliseconds \t Time interval between two sending of messages by a single client",
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
            chrono::milliseconds(parser.getoptIntRequired('i')),
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
    std::latch broadcastBegin(param.nbClients);
    std::latch broadcastEnd(param.nbClients);
    std::latch ackDisconnectIntent(param.nbClients);

    // We launch all the clients to make the experiment
    vector<jthread> clients(param.nbClients);
    for (auto& c : clients) {
        c = jthread(client, std::ref(param), std::ref(measures), std::ref(broadcastBegin), std::ref(broadcastEnd), std::ref(ackDisconnectIntent));
    }
    for (auto& c: clients)
        c.join();

    // Display statistics
    measures.rtts.resize(measures.nbRtts);
    std::ranges::sort(measures.rtts);
    cout << "Ratio received / sent messages = " << measures.nbRtts << " / " << param.nbMsg * param.nbClients << " (" << measures.nbRtts * 100 / param.nbMsg / param.nbClients << "%)\n";
    cout << "Average = " << (std::reduce(measures.rtts.begin(), measures.rtts.end()) / measures.rtts.size()).count() << " ms\n";
    cout << "Min = " << measures.rtts[0].count() << " ms\n";
    cout << "Q1 = " << measures.rtts[measures.rtts.size()/4].count() << " ms\n";
    cout << "Q2 = " << measures.rtts[measures.rtts.size()/2].count() << " ms\n";
    cout << "Q3 = " << measures.rtts[measures.rtts.size()*3/4].count() << " ms\n";
    cout << "Centile #99 = " << measures.rtts[measures.rtts.size()*99/100].count() << " ms\n";
    cout << "Max = " << measures.rtts[measures.rtts.size()-1].count() << " ms\n";

    return 0;
}
