#pragma once

class IServerEventsListener {
public:
    virtual void onEvent(const UdpClient &client, NetworkEvent::Event event) = 0;

    virtual ~IServerEventsListener() = default;
};