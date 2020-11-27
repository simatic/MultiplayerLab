#pragma once
#include "State.h"
#include "World.h"
#include "GameClient.h"
#include "GameServer.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class MultiplayerGameState : public State
{
public:
	MultiplayerGameState(StateStack& stack, Context context, bool isHost);

	virtual void		draw();
	virtual bool		update(sf::Time dt);
	virtual bool		tick() override;
	virtual bool		handleEvent(const sf::Event& event);


private:
	World				mWorld;
	sf::Clock			mClock;

	std::unique_ptr<GameServer> mGameServer;
	GameClient			mGameClient;
	
	bool				mIsHost;
	bool				mConnected;




};