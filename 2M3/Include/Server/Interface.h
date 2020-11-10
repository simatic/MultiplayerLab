#pragma once
#include <SFML/Window.hpp>
#include <imgui.h>
#include "Server/ServerNetworkHandling.h"

class Interface {

    struct CompiledEvents {
        std::vector<float> timestamps;
        std::vector<float> values;
        std::vector<sf::Uint64> packetIndices;
    };

    struct PacketLifecycle {
        const sf::Uint64 packetIndex;
        std::pair<sf::Vector2f, sf::Vector2f> edges;
    };

    typedef std::map<NetworkEvent::Type, CompiledEvents> CompiledEventsMap;


private:
    static std::map<ClientID, CompiledEventsMap> clientEvents;
    static std::map<ClientID, std::vector<PacketLifecycle>> clientPacketLifecycles;

    static const PacketLifecycle* getClosest(const std::vector<PacketLifecycle>& lifecycles, float x, float y);

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
