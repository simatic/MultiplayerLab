#pragma once

#include <string>
#include <boost/asio.hpp>

void tcp_send(boost::asio::ip::tcp::socket *psock, std::string const& s);
std::string tcp_receive(boost::asio::ip::tcp::socket *psock);