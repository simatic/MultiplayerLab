#include "State.h"
#include <Client/Managers/ClientGameManager.h>
#include <Client/Modules/ClientNetworkModule.h>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>


class GameState : public State
{
public:
						GameState(StateStack& stack, Context context);

	virtual void		draw();
	virtual bool		update(sf::Time dt);
	virtual bool		handleEvent(const sf::Event& event);

private:
	std::unique_ptr<ClientNetworkModule> networkModule;
	std::unique_ptr<ClientGameManager> gameManager;
	sf::RenderTarget*	target = nullptr;
};