#pragma once
#include <SFML/Window.hpp>
#include <imgui.h>
#include <thread>
#include <SFML/Graphics/RenderWindow.hpp>
#include <mutex>
#include "Server/NetworkTypes.h"
#include "Server/ServerNetworkHandler.h"

class Interface {

    struct PacketInfo {
        PacketSequenceIndex sequenceIndex;
        NetworkEvent::Type packetType;
        float timestamp;
    };

    struct PacketLifecycle {
        PacketSequenceIndex sequenceIndex;

        // Time packet A -> Time packet B
        float timestampA;
        float timestampB;
    };

    struct CompiledEvents {
        // values for packets received on the server for this client
        std::vector<float> receivedTimestamps;
        std::vector<PacketSequenceIndex> receivedPacketIndices;

        // values for packets sent from the server, after artificial delay
        std::vector<float> sentTimestamps;
        std::vector<PacketSequenceIndex> sentPacketIndices;

        // Represents the link between a packet sent by a client and its delayed counter-part
        std::vector<PacketLifecycle> receptionLinks{};

        // Represents the link between a packet sent by the server and its delayed counter-part
        std::vector<PacketLifecycle> transmissionLinks{};
    };

private:
    std::map<ClientID, CompiledEvents> clientEvents{};

    // values for packets received on the server for this client, after artificial delay
    std::vector<float> afterDelayTimestamps;
    std::vector<PacketSequenceIndex> afterDelayPacketIndices;

    // values for packets sent from the server for this client
    std::vector<float> sendingStartsTimestamps;
    std::vector<PacketSequenceIndex> sendingStartsPacketIndices;

    std::mutex linkAccess{};

    ServerNetworkHandler& serverNetwork;
    /// Has ImPlot being initialized yet?
    bool implotInit = false;
    sf::Clock deltaClock{};

private:

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
