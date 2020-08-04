#include "State.h"
//#include <Book/World.hpp>
//#include <Book/Player.hpp>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>


class GameState : public State
{
	public:
							GameState(StateStack& stack, Context context);

//		virtual void		draw();
//		virtual bool		update(sf::Time dt);
//		virtual bool		handleEvent(const sf::Event& event);


	private:
//		World				mWorld;
//		Player&				mPlayer;
};