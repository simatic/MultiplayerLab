#include <iostream>
#include <Server/ServerNetworkHandler.h>
#include <Server/ServerClock.h>
#include "Server/Server.h"
#include "Server/DelayCreation.h"

DelayCreator::DelayCreator(ServerNetworkHandler& serverNetworkHandler): serverNetwork(serverNetworkHandler) {
    backingThread = std::thread([&](){this->threadCode();});
}

void DelayCreator::threadCode() {
    std::vector<std::unique_ptr<PacketWithDelay>> packetWithDelayList;
    sf::UdpSocket& socket = serverNetwork.getSocket();
    sf::Time lastTime = ServerClock::getInstance().get();
    sf::Time time = ServerClock::getInstance().get();
    sf::Time deltaTime = time - lastTime;

    while(!stopServer) {
        // TODO: utiliser des guard plutôt que lock/unlock
        mutex4Packet4Delay.lock();
        while (!packet4DelayList.empty()){
            auto& packetToDelay = packet4DelayList[0];
            float incomingDelayToApply = packetToDelay->client.settings.getIncomingDelay();
            auto packet = std::make_unique<PacketWithDelay>(std::move(packetToDelay->logicalPacket), packetToDelay->client, incomingDelayToApply);
            packetWithDelayList.push_back(std::move(packet));
            packet4DelayList.erase(packet4DelayList.begin());
        }
        mutex4Packet4Delay.unlock();
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
                    serverNetwork.triggerEvent(client, NetworkEvent::Event{ServerClock::getInstance().get(), NetworkEvent::Type::PacketDelayed,
                                                                                   packet->logicalPacket->getSequenceIndex()});
                    auto response = packet->logicalPacket->handle(serverNetwork.getNetworkModule());
                    if(response) {
                        serverNetwork.triggerEvent(client, NetworkEvent::Event{ServerClock::getInstance().get(), NetworkEvent::Type::SendingPacket,
                                                                                       response->getSequenceIndex()});
                        float outgoingDelayToApply = client.settings.getOutgoingDelay();
                        mutex4ResponsePacketWithDelay.lock();
                        responsePacketWithDelayList.push_back(std::make_unique<PacketWithDelay>(std::move(response), client, outgoingDelayToApply));
                        mutex4ResponsePacketWithDelay.unlock();
                    }
                }
                it = packetWithDelayList.erase(it);
            }
            else{
                it++;
            }
        }
        mutex4ResponsePacketWithDelay.lock();
        for(auto it = responsePacketWithDelayList.begin(); it != responsePacketWithDelayList.end();){
            auto& packet = *it;
            auto& client = packet->client;
            packet->delay -= deltaTime.asSeconds();
            if(packet->delay <= 0){
                if(!client.settings.outGoingPacketLost()){
                    serverNetwork.triggerEvent(client, NetworkEvent::Event{ServerClock::getInstance().get(), NetworkEvent::Type::SentPacket,
                                                                                   packet->logicalPacket->getSequenceIndex()});
                    packet->logicalPacket->realSend(socket, client.address, client.port);
                }
                it = responsePacketWithDelayList.erase(it);
            }
            else{
                it++;
            }
        }
        mutex4ResponsePacketWithDelay.unlock();
    }
}

void DelayCreator::sendToClient(const UdpClient& client, std::unique_ptr<Packet>&& packet) {
    mutex4ResponsePacketWithDelay.lock();
    responsePacketWithDelayList.push_back(
            std::make_unique<PacketWithDelay>(std::move(packet), client, client.settings.getOutgoingDelay()));
    mutex4ResponsePacketWithDelay.unlock();
}

bool DelayCreator::isRunning() {
    return !stopServer;
}

void DelayCreator::delayReceivedPacket(UdpClient& client, std::unique_ptr<Packet>&& packet) {
    mutex4Packet4Delay.lock();
    packet4DelayList.push_back(std::make_unique<PacketToAddDelayTo>(std::move(packet), client));
    mutex4Packet4Delay.unlock();
}

void DelayCreator::join() {
    stopServer = true;
    backingThread.join();
}

