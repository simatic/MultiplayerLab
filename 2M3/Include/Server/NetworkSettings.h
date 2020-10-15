#pragma once

#include <random>

class NetworkSettings{
private:
    float percentageOutGoingPacketLost;
    float percentageInComingPacketLost;

public:
    static NetworkSettings* instance;

private:
    NetworkSettings();

public:
    static NetworkSettings* getInstance();

    float getPercentageOutGoingPacketLost() const;
    void setPercentageOutGoingPacketLost(float percentage);
    float getPercentageInComingPacketLost() const;
    void setPercentageInComingPacketLost(float percentageInComingPacketLost);

    bool outGoingPacketLost();
    bool inComingPacketLost();
};
