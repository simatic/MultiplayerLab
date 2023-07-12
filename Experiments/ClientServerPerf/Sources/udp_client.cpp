//
// Application to make statistics on tcp
// inspired by blocking_tcp_echo_client.cpp (from https://www.boost.org/doc/libs/1_35_0/doc/html/boost_asio/example/echo/blocking_tcp_echo_client.cpp)
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//

#include <boost/asio.hpp>
#include <iostream>
#include <latch>
#include <thread>
#include <vector>
#include "common.h"
#include "Measures.h"
#include "options.h"
#include "OptParserExtended.h"
#include "udp_communication.h"

using boost::asio::ip::udp;
using namespace std;
using namespace mlib;

struct Param {
    string host;
    string port;
    int nbMsg{0};
    int sizeMsg{0};
    int nbClients{0};
    bool verbose{false};

    static std::string csvHeadline()
    {
        return std::string { "host,port,nbMsg,sizeMsg,nbClients"};
    }

    [[nodiscard]] std::string asCsv() const
    {
        return std::string { host + "," + port + "," + std::to_string(nbMsg) + "," + std::to_string(sizeMsg) + "," + std::to_string(nbClients) };
    }
};

unsigned int sendMsgToBroadcast(udp::socket &sock, udp::endpoint const&serverEndpoint, Param const &param, unsigned char myId, unsigned int msgNum)
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
    sendPacket(sock, s_cmtb, serverEndpoint);
    return msgNum;
}

// Should return true if handlePacket discovers client will not receive any more packets.
bool handlePacket(udp::socket &sock, udp::endpoint const&serverEndpoint, const string &msgString, Param const &param,
             Measures &measures, std::latch &allAckDisconnectIntentReceived)
{
    static thread_local unsigned char myId{0};
    static thread_local unsigned int msgNum{0};
    switch (ServerMsgId serverMsgId{ static_cast<ServerMsgId>(msgString[0]) }; serverMsgId)
    {
        case ServerMsgId::AckDisconnectIntent :
            allAckDisconnectIntentReceived.count_down();
            return true;
        case ServerMsgId::AckDoneSendingMessages :
            return false;
        case ServerMsgId::BroadcastBegin :
        {
            msgNum = sendMsgToBroadcast(sock, serverEndpoint, param, myId, msgNum);
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
                    msgNum = sendMsgToBroadcast(sock, serverEndpoint, param, myId, msgNum);
                }
                else
                {
                    auto cdsm {serializeStruct<ClientDoneSendingMessages>(ClientDoneSendingMessages{ClientMsgId::DoneSendingMessages, myId})};
                    sendPacket(sock, cdsm, serverEndpoint);
                }
            }
            return false;
        }
        case ServerMsgId::BroadcastEnd :
        {
            auto cdi {serializeStruct<ClientDisconnectIntent>(ClientDisconnectIntent{ClientMsgId::DisconnectIntent, myId})};
            sendPacket(sock, cdi, serverEndpoint);
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

void msgReceive(udp::socket &sock, Param const& param, Measures & measures, std::latch &allAckDisconnectIntentReceived)
{
    for (;;)
    {
        udp::endpoint serverEndpoint; // It is always the server endpoint
        auto msgString{receivePacket(sock, serverEndpoint)};
        if (handlePacket(sock, serverEndpoint, msgString, param, measures, allAckDisconnectIntentReceived))
            break;
    }
}

void client(Param const& param, Measures & measures, std::latch &allAckDisconnectIntentReceived)
{
    boost::asio::io_service ioService;

    udp::socket sock(ioService, udp::endpoint(udp::v4(), 0));

    udp::resolver resolver(ioService);
    udp::resolver::query query(udp::v4(), param.host, param.port);
    udp::resolver::iterator iterator = resolver.resolve(query);
    udp::endpoint serverEndpoint = *iterator;

    unsigned char myId{ 0 };

    // Create a thread for receiving data
    auto t = jthread(msgReceive, std::ref(sock), std::ref(param), std::ref(measures),
                     std::ref(allAckDisconnectIntentReceived));

    // Send IdRequest to server
    auto cir {serializeStruct<ClientIdRequest>(ClientIdRequest{ClientMsgId::IdRequest, param.nbClients})};
    sendPacket(sock, cir, serverEndpoint);

    allAckDisconnectIntentReceived.wait();

    // msgReceive thread will exit when it has received ServerMsgId::AckDisconnectIntent)
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
            "s:size size_in_bytes \t Size of messages sent by a client (must be in interval [22,65515])",
            "c:clients number \t Number of clients which send messages to server",
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
            parser.getoptStringRequired('p'),
            parser.getoptIntRequired('n'),
            parser.getoptIntRequired('s'),
            parser.getoptIntRequired('c'),
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
    cout << "udp," << param.asCsv() << "," << measures.asCsv(param.nbMsg, param.nbClients) << "\n";

    return 0;
}
