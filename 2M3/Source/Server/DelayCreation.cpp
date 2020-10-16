#include <iostream>
#include "Server/ServerMain.h"
#include "Server/NetworkSettings.h"
#include "Server/DelayCreation.h"


void delayThread(sf::UdpSocket* socketPtr){
    std::vector<packetWithDelay> packetWithDelayList;
    auto instanceNetworkSetting = NetworkSettings::getInstance();
    sf::UdpSocket& socket = *socketPtr;
    sf::Clock clock;
    sf::Time lastTime = clock.getElapsedTime();
    sf::Time time = clock.getElapsedTime();
    sf::Time deltaTime = time - lastTime;
    while(true){
        Delay::mutex4Packet4Delay.lock();
        while (!Delay::packet4DelayList.empty()){
            packetWithDelay packet;
            packet.logicalPacket = std::move(Delay::packet4DelayList[0].logicalPacket);
            packet.remoteAddress = Delay::packet4DelayList[0].remoteAddress;
            packet.remotePort = Delay::packet4DelayList[0].remotePort;
            packet.delayMilliseconds = 1000;
            packetWithDelayList.push_back(std::move(packet));
            Delay::packet4DelayList.erase(Delay::packet4DelayList.begin());
        }
        Delay::mutex4Packet4Delay.unlock();
        time = clock.getElapsedTime();
        deltaTime = time - lastTime;
        lastTime = time;
        for (auto it = packetWithDelayList.begin(); it!=packetWithDelayList.end();){
            packetWithDelay& packet = *it;
            packet.delayMilliseconds -= deltaTime.asMilliseconds();
            if(packet.delayMilliseconds <= 0){
                std::cout << "[Debug] Received packet with ID " << packet.logicalPacket->getID() << std::endl;
                if(!instanceNetworkSetting->inComingPacketLost()){
                    auto response = packet.logicalPacket->handle();
                    if(response) {
                        if(!instanceNetworkSetting->outGoingPacketLost()){
                            response->send(socket, packet.remoteAddress, packet.remotePort);
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

