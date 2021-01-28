#pragma once

#include <memory>
#include <vector>
#include <SFML/Network.hpp>
#include "Common/Network/NetworkHandler.h"
#include "Server/NetworkSettings.h"
#include "Server/NetworkTypes.h"

class Interface;
class DelayCreator;
class IServerEventsListener;
template <class PacketType>
class Buffer;

class ServerNetworkHandler: public NetworkHandler {
private:
    // TODO: custom
    constexpr static float DelayBeforeDeconnection = 5.0f;
    INetworkModule* networkModule = nullptr;

    std::string ip;
    unsigned short port;
    sf::UdpSocket socket;
    ClientID currentClientID = 0;
    std::vector<std::shared_ptr<UdpClient>> clients{};
    std::map<ClientID, float> lastPacketTimes{};
    bool running = true;
    PacketSequenceIndex sequenceIndex = 0;
    std::vector<IServerEventsListener*> listeners{};
    std::shared_ptr<DelayCreator> delayCreator = nullptr;

protected:
    PacketSequenceIndex generateNextIndex() override;

public:
    ServerNetworkHandler(Buffer<Packet>& buffer, const std::string& ip, unsigned short port);

    sf::UdpSocket& getSocket();

    const std::string& getIP() const;
    const unsigned short getPort() const;

    /// Returns the client with the given address and port.
    /// Creates a new client if none match.
    UdpClient& getOrCreateClient(sf::IpAddress address, unsigned short port);

    /// Send a packet to all clients
    void broadcast(std::unique_ptr<Packet> toBroadcast);

    const std::vector<std::shared_ptr<UdpClient>>& getClients() {
        return clients;
    }

    void triggerEvent(const UdpClient& client, NetworkEvent::Event event);

    void updateNonConnectedClients();
    void updateLastPacketTime(const UdpClient& client);

    void killNetworkThread();

    void registerListener(IServerEventsListener* listener);

    void sendToClient(const UdpClient& client, std::unique_ptr<Packet>&& packet);

    DelayCreator& getDelayCreator();

    bool isRunning();

    INetworkModule* getNetworkModule() const;

    void setNetworkModule(INetworkModule* networkModule);
};

