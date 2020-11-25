#pragma once
#include <SFML/Window.hpp>
#include <imgui.h>
#include "Server/ServerNetworkHandling.h"

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
        const PacketSequenceIndex sequenceIndex;
        std::pair<sf::Vector2f, sf::Vector2f> edges;
    };

    typedef std::map<NetworkEvent::Type, CompiledEvents> CompiledEventsMap;


private:
    static std::map<ClientID, CompiledEventsMap> clientEvents;
    static std::map<ClientID, std::vector<PacketLifecycle>> clientPacketLifecycles;

    /// Gets the closest packet and packet lifecyle to the mouse.
    /// Returns a PacketInfo with sequenceIndex = 0 if no packet is close
    /// Returns a PacketLifecycle* set to nullptr if no lifecycle is close, or the closest lifecycle is not linked to the closest packet
    /// \param packets
    /// \param lifecycles
    /// \param x
    /// \param y
    /// \return
    static std::pair<const PacketInfo, const PacketLifecycle*> getClosest(const CompiledEventsMap& packets, const std::vector<PacketLifecycle>& lifecycles, float x, float y);

public:
    static void pollEvents(sf::Window& window);
    static void render();
    static void renderClientWindow(const std::string& name, UdpClient& client);

    static void onEvent(const UdpClient &client, NetworkEvent::Event event);

    static void linkPackets(const UdpClient &client, const NetworkEvent::Event &event,
                            const std::map<NetworkEvent::Type, Interface::CompiledEvents> &eventMap,
                            const NetworkEvent::Type typeToLinkTo);

    static void renderGraph(const UdpClient &client);
};
