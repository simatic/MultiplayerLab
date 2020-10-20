#pragma once

#include <random>

class NetworkSettings{
private:
    float percentageOutGoingPacketLost;
    float percentageInComingPacketLost;
    float incomingDelay;
    float outgoingDelay;

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
