#pragma once

#include <string>
#include <boost/asio.hpp>

void sendPacket(boost::asio::ip::tcp::socket *psock, std::string const& s);
std::string receivePacket(boost::asio::ip::tcp::socket *psock);