#pragma once

#include <string>
#include <boost/asio.hpp>

void sendPacket(boost::asio::ip::udp::socket & sock, std::string const& s, boost::asio::ip::udp::endpoint const& endpoint);
std::string receivePacket(boost::asio::ip::udp::socket & sock, boost::asio::ip::udp::endpoint & senderEndpoint);