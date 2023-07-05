#include <iostream>
#include "udp_communication.h"
#include "common.h"

void sendPacket(boost::asio::ip::udp::socket & sock, std::string const& s, boost::asio::ip::udp::endpoint const& endpoint)
{
    sock.send_to(boost::asio::buffer(s.data(), s.length()), endpoint);
}

std::string receivePacket(boost::asio::ip::udp::socket & sock, boost::asio::ip::udp::endpoint & senderEndpoint)
{
    // We read the message itself
    std::vector<char> v(maxLength);
    auto length = sock.receive_from(boost::asio::buffer(v), senderEndpoint);
    return std::string{v.data(), length};
}