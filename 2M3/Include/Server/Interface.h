#pragma once
#include <SFML/Window.hpp>
#include <imgui.h>
#include <thread>
#include <SFML/Graphics/RenderWindow.hpp>
#include "Server/NetworkTypes.h"
#include "Server/ServerNetworkHandler.h"

class Interface {

    struct PacketInfo {
        PacketSequenceIndex sequenceIndex;
        NetworkEvent::Type packetType;
        float timestamp;
    };

    struct CompiledEvents {
        // values for packets received on the server for this client
        std::vector<float> receivedTimestamps;
        std::vector<PacketSequenceIndex> receivedPacketIndices;

        // values for packets sent from the server, after artificial delay
        std::vector<float> sentTimestamps;
        std::vector<PacketSequenceIndex> sentPacketIndices;
    };

    // values for packets received on the server for this client, after artificial delay
    std::vector<float> afterDelayTimestamps;
    std::vector<PacketSequenceIndex> afterDelayPacketIndices;

    // values for packets sent from the server for this client
    std::vector<float> sendingStartsTimestamps;
    std::vector<PacketSequenceIndex> sendingStartsPacketIndices;

    struct PacketLifecycle {
        PacketSequenceIndex sequenceIndex;
        std::pair<sf::Vector2f, sf::Vector2f> edges;
    };


private:
    std::map<ClientID, CompiledEvents> clientEvents{};
    std::map<ClientID, bool> pauseGraphForClient{};
    std::map<ClientID, std::vector<PacketLifecycle>> clientPacketLifecycles{};
    ServerNetworkHandler& serverNetwork;
    /// Has ImPlot being initialized yet?
    bool implotInit = false;
    sf::Clock deltaClock{};

private:

    void linkPackets(const UdpClient &client, const NetworkEvent::Event &event,
                            const std::map<NetworkEvent::Type, Interface::CompiledEvents> &eventMap,
                            const NetworkEvent::Type typeToLinkTo);

    void renderIncomingPackets(float clientWidth, float height);
    void renderOutcomingPackets(float clientWidth, float height);
    void renderIncomingGraph();
    void renderOutgoingGraph();

public:
    explicit Interface(ServerNetworkHandler& handler);

    void onEvent(const UdpClient &client, NetworkEvent::Event event);
    void render(sf::RenderWindow& renderTarget, float clientWidth, float interfaceHeight, float startY);

    ~Interface();
};
