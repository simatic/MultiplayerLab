#include <iostream>
#include "Server/NetworkSettings.h"

NetworkSettings* NetworkSettings::instance = nullptr;

NetworkSettings::NetworkSettings() {
    percentageOutGoingPacketLost = 0.5;
    percentageInComingPacketLost = 0.5;
}

NetworkSettings* NetworkSettings::getInstance(){
    if(!instance){
        instance = new NetworkSettings();
    }
    return instance;
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


bool NetworkSettings::outGoingPacketLost(){
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> dist(0,1);
    return dist(gen) < percentageOutGoingPacketLost;
}

bool NetworkSettings::inComingPacketLost(){
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> dist(0,1);
    return dist(gen) < percentageInComingPacketLost;
}

