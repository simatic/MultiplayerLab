#include "Common/Network/NetworkHandler.h"

sf::UdpSocket& NetworkHandler::getSocket() {
	return socket;
}