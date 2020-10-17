#ifndef BOOK_APPLICATION_HPP
#define BOOK_APPLICATION_HPP

#include <vector>

#include "ResourceHolder.h"
#include "ResourceIdentifiers.h"
#include "KeyBinding.h"
#include "StateStack.h"
#include "Client.h"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>


class Application
{
	public:
								Application();
		void					run();
		

	private:
		void launchClientThread();
		/*void					processInput();
		void					update(sf::Time dt);
		void					tick();
		void					render();

		void					updateStatistics(sf::Time dt);
		void					registerStates();*/


	private:

		sf::RenderWindow		mWindow;
		/*TextureHolder			mTextures;
	  	FontHolder				mFonts;*/

		KeyBinding				mKeyBinding1;
		KeyBinding				mKeyBinding2;
		//StateStack				mStateStack;
		int _clientCount;
		sf::Mutex _mutex;

		/*sf::Text				mStatisticsText;
		sf::Time				mStatisticsUpdateTime;
		std::size_t				mStatisticsNumFrames;*/
};

#endif // BOOK_APPLICATION_HPP
