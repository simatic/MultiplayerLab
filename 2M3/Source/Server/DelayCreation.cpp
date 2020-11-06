#include <iostream>
#include <Server/ServerNetworkHandling.h>
#include "Server/ServerMain.h"
#include "Server/NetworkSettings.h"
#include "Server/DelayCreation.h"


[[noreturn]] void delayThread(sf::UdpSocket* socketPtr){
    std::vector<std::unique_ptr<packetWithDelay>> packetWithDelayList;
    sf::UdpSocket& socket = *socketPtr;
    sf::Clock clock;
    sf::Time lastTime = clock.getElapsedTime();
    sf::Time time = clock.getElapsedTime();
    sf::Time deltaTime = time - lastTime;
    while(true) {
        Delay::mutex4Packet4Delay.lock();
        while (!Delay::packet4DelayList.empty()){
            auto& packetToDelay = Delay::packet4DelayList[0];
            float incomingDelayToApply = packetToDelay->client.settings.getIncomingDelay();
            float outgoingDelayToApply = packetToDelay->client.settings.getOutgoingDelay();
            auto packet = std::make_unique<packetWithDelay>(std::move(packetToDelay->logicalPacket), packetToDelay->client, incomingDelayToApply, outgoingDelayToApply);
            packetWithDelayList.push_back(std::move(packet));
            Delay::packet4DelayList.erase(Delay::packet4DelayList.begin());
        }
        Delay::mutex4Packet4Delay.unlock();
        time = clock.getElapsedTime();
        deltaTime = time - lastTime;
        lastTime = time;
        for (auto it = packetWithDelayList.begin(); it!=packetWithDelayList.end();){
            auto& packet = *it;
            packet->incomingDelay -= deltaTime.asSeconds();
            packet->outgoingDelay -= deltaTime.asSeconds();
            if(packet->incomingDelay <= 0){
                std::cout << "[Debug] Received packet with ID " << packet->logicalPacket->getID() << std::endl;
                auto& client = packet->client;
                if(!client.settings.inComingPacketLost()){
                    ServerNetworkHandling::triggerEvent(client, NetworkEvent::Event{clock.getElapsedTime(), NetworkEvent::Type::PacketDelayed, packet->logicalPacket->getIndex()});
                    auto response = packet->logicalPacket->handle();
                    if(response) {
                        // TODO: outgoing delay
                        if(!client.settings.outGoingPacketLost()){
                            response->send(socket, client.address, client.port);
                        }
                    }
                }
                it = packetWithDelayList.erase(it);
            }
            else{
                it++;
            }
        }
    }
}

