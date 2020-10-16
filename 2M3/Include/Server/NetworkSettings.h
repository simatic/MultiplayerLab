#pragma once

#include <random>

class NetworkSettings{
private:
    float percentageOutGoingPacketLost;
    float percentageInComingPacketLost;

public:
    NetworkSettings();

    float getPercentageOutGoingPacketLost() const;
    void setPercentageOutGoingPacketLost(float percentage);
    float getPercentageInComingPacketLost() const;
    void setPercentageInComingPacketLost(float percentageInComingPacketLost);

    bool outGoingPacketLost();
    bool inComingPacketLost();
};
