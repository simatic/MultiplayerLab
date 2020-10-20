#include <iostream>
#include "Server/NetworkSettings.h"

NetworkSettings::NetworkSettings() {
    percentageOutGoingPacketLost = 0.0;
    percentageInComingPacketLost = 0.0;
    incomingDelay = 0.0;
    outgoingDelay = 0.0;
}

float NetworkSettings::getPercentageOutGoingPacketLost() const {
    return percentageOutGoingPacketLost;
}

void NetworkSettings::setPercentageOutGoingPacketLost(float percentage) {
    if(percentage <= 1 && percentage >= 0){
        NetworkSettings::percentageOutGoingPacketLost = percentage;
    }
}

float NetworkSettings::getPercentageInComingPacketLost() const {
    return percentageInComingPacketLost;
}

void NetworkSettings::setPercentageInComingPacketLost(float percentage) {
    if(percentage <= 1 && percentage >= 0) {
        NetworkSettings::percentageInComingPacketLost = percentage;
    }
}


bool NetworkSettings::outGoingPacketLost() const {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> dist(0,1);
    return dist(gen) < percentageOutGoingPacketLost;
}

bool NetworkSettings::inComingPacketLost() const {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> dist(0,1);
    return dist(gen) < percentageInComingPacketLost;
}

float NetworkSettings::getIncomingDelay() const {
    return incomingDelay;
}

float NetworkSettings::getOutgoingDelay() const {
    return outgoingDelay;
}

void NetworkSettings::setIncomingDelay(float delay) {
    incomingDelay = delay;
}

void NetworkSettings::setOutgoingDelay(float delay) {
    outgoingDelay = delay;
}

