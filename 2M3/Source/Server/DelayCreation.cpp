#include <iostream>
#include <Server/ServerNetworkHandling.h>
#include <Server/ServerClock.h>
#include "Server/ServerMain.h"
#include "Server/DelayCreation.h"


[[noreturn]] void delayThread(sf::UdpSocket* socketPtr){
    std::vector<std::unique_ptr<packetWithDelay>> packetWithDelayList;
    sf::UdpSocket& socket = *socketPtr;
    sf::Time lastTime = ServerClock::getInstance().get();
    sf::Time time = ServerClock::getInstance().get();
    sf::Time deltaTime = time - lastTime;

    std::vector<std::unique_ptr<packetWithDelay>> reponsePacketWithDelayList;
    while(true) {
        Delay::mutex4Packet4Delay.lock();
        while (!Delay::packet4DelayList.empty()){
            auto& packetToDelay = Delay::packet4DelayList[0];
            float incomingDelayToApply = packetToDelay->client.settings.getIncomingDelay();
            auto packet = std::make_unique<packetWithDelay>(std::move(packetToDelay->logicalPacket), packetToDelay->client, incomingDelayToApply);
            packetWithDelayList.push_back(std::move(packet));
            Delay::packet4DelayList.erase(Delay::packet4DelayList.begin());
        }
        Delay::mutex4Packet4Delay.unlock();
        time = ServerClock::getInstance().get();
        deltaTime = time - lastTime;
        lastTime = time;
        for (auto it = packetWithDelayList.begin(); it!=packetWithDelayList.end();){
            auto& packet = *it;
            packet->delay -= deltaTime.asSeconds();
            if(packet->delay <= 0){
                std::cout << "[Debug] Received packet with ID " << packet->logicalPacket->getID() << std::endl;
                auto& client = packet->client;
                if(!client.settings.inComingPacketLost()){
                    ServerNetworkHandling::triggerEvent(client, NetworkEvent::Event{ServerClock::getInstance().get(), NetworkEvent::Type::PacketDelayed, packet->logicalPacket->getIndex()});
                    auto response = packet->logicalPacket->handle();
                    if(response) {
                        // TODO: outgoing delay
                        float outgoingDelayToApply = client.settings.getOutgoingDelay();
                        reponsePacketWithDelayList.push_back(std::make_unique<packetWithDelay>(std::move(response), client, outgoingDelayToApply));
                    }
                }
                it = packetWithDelayList.erase(it);
            }
            else{
                it++;
            }
        }
        for(auto it = reponsePacketWithDelayList.begin(); it != reponsePacketWithDelayList.end();){
            auto& packet = *it;
            auto& client = packet->client;
            packet->delay -= deltaTime.asSeconds();
            if(packet->delay <= 0){
                if(!client.settings.outGoingPacketLost()){
                    packet->logicalPacket->send(socket, client.address, client.port);
                }
                it = reponsePacketWithDelayList.erase(it);
            }
            else{
                it++;
            }
        }
    }
}

