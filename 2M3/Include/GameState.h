#include "State.h"
#include "Common/Managers/GameManager.h"

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
	GameManager*		gameManager;
	sf::RenderTarget*	target;

    void createWall(sf::Vector2f axis) const;
};