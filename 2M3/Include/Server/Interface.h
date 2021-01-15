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
        std::vector<float> timestamps;
        std::vector<float> values;
        std::vector<PacketSequenceIndex> packetIndices;
    };

    struct PacketLifecycle {
        PacketSequenceIndex sequenceIndex;
        std::pair<sf::Vector2f, sf::Vector2f> edges;
    };

    typedef std::map<NetworkEvent::Type, CompiledEvents> CompiledEventsMap;


private:
    std::map<ClientID, CompiledEventsMap> clientEvents{};
    std::map<ClientID, bool> pauseGraphForClient{};
    std::map<ClientID, std::vector<PacketLifecycle>> clientPacketLifecycles{};
    ServerNetworkHandler& serverNetwork;
    /// Has ImPlot being initialized yet?
    bool implotInit = false;
    sf::Clock deltaClock{};

    /// Gets the closest packet and packet lifecyle to the mouse.
    /// Returns a PacketInfo with sequenceIndex = 0 if no packet is close
    /// Returns a PacketLifecycle* set to nullptr if no lifecycle is close, or the closest lifecycle is not linked to the closest packet
    /// \param packets
    /// \param lifecycles
    /// \param x
    /// \param y
    /// \return
    std::pair<const PacketInfo, const PacketLifecycle*> getClosest(const CompiledEventsMap& packets, const std::vector<PacketLifecycle>& lifecycles, float x, float y);

private:
    void renderClientWindow(const std::string& name, UdpClient& client);

    void linkPackets(const UdpClient &client, const NetworkEvent::Event &event,
                            const std::map<NetworkEvent::Type, Interface::CompiledEvents> &eventMap,
                            const NetworkEvent::Type typeToLinkTo);

    void renderGraph(const UdpClient &client);

public:
    explicit Interface(ServerNetworkHandler& handler);

    void onEvent(const UdpClient &client, NetworkEvent::Event event);
    void render(sf::RenderWindow& renderTarget, float clientWidth, float clientHeight, float startY);

    ~Interface();
};
