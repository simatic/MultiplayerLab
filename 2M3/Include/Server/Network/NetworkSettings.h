#pragma once

#include <random>

class NetworkSettings{
private:
    float percentageOutGoingPacketLost = 0.0f;
    float percentageInComingPacketLost = 0.0f;
    float incomingDelay = 0.0f;
    float outgoingDelay = 0.0f;

public:
    NetworkSettings();

    float getPercentageOutGoingPacketLost() const;
    void setPercentageOutGoingPacketLost(float percentage);
    float getPercentageInComingPacketLost() const;
    void setPercentageInComingPacketLost(float percentageInComingPacketLost);

    float getIncomingDelay() const;
    float getOutgoingDelay() const;
    void setIncomingDelay(float delay);
    void setOutgoingDelay(float delay);

    bool outGoingPacketLost() const;
    bool inComingPacketLost() const;
};
