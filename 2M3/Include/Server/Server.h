#pragma once

#include <iostream>
#include <string>
#include <SFML/Network.hpp>
#include <GameManager.h>
#include <Server/Modules/ServerNetworkModule.h>
#include "Common/Network/Network.h"
#include "ServerNetworkHandler.h"
#include "ServerNetworkThread.h"
#include "IServerEventsListener.h"

class Server: public IServerEventsListener {
private:
    GameManager game;
    std::thread gameThread;
    /// Mapping between the client ID and its entity
    std::unordered_map<ClientID, std::uint32_t> playerEntityIDs{};
    std::mutex playerConnection{};

public:
    explicit Server(const std::string& ip, unsigned short port);

    void run();
    void stop();
    bool isRunning();
    bool isReady();
    ServerNetworkHandler& getNetworkHandler();
    ServerNetworkModule& getNetworkModule();

    void onEvent(const UdpClient& client, NetworkEvent::Event event) override;

    ~Server();

private:
    void createWall(float x, float y, float angle);
    void initGame();
    void runGame();
    void makePlayerJoin(const UdpClient& client);
    void sendWorldStateTo(const UdpClient& client);
};