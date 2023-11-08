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

void analyzePacket(udp::socket &sock, udp::endpoint const& senderEndpoint, const string &msgString, Param const& param)
{
    switch (ClientMsgId clientMsgTyp{ static_cast<ClientMsgId>(msgString[0]) }; clientMsgTyp)
    {
        case ClientMsgId::DoneSendingMessages :
        {
            auto s{serializeStruct<ServerAckDoneSendingMessages>(ServerAckDoneSendingMessages{ServerMsgId::AckDoneSendingMessages})};
            sendPacket(sock, s, senderEndpoint);
            if (param.verbose)
                cout << "A client is done sending messages\n";
            break;
        }
        case ClientMsgId::Ping :
        {
            auto cmtb{deserializeStruct<ClientPing>(msgString)};
            auto s {serializeStruct<ServerPingResponse>(ServerPingResponse{ServerMsgId::PingResponse,
                                                                                   cmtb.msgNum,
                                                                                   cmtb.sendTime, cmtb.filler})};
            if (param.verbose)
                cout << "Received a ping message ==> Respond to this message\n";
            sendPacket(sock, s, senderEndpoint);
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
