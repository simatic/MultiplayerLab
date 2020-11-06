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
		void run();
		

	private:
		void launchClientThread();
		void addClientThread();
		void terminateClientThreads();


	private:
		// Main window in which the Clients' views will be rendered.
		sf::RenderWindow		mWindow;

		// Vector of Clients.
		std::vector<Client*> _clients;

		// Number of Clients in _clients.
		int _clientCount;

		// Thread : _clientThreads[i] corresponds to the Client : _clients[i] 
		std::vector<sf::Thread*> _clientThreads;

		// Common mutex between all the threads to lock variables.
		sf::Mutex _mutex;

		/* Index of the last launched Thread to keep a track of launched Threads
		 * and not yet launched Threads.
		*/
		int _clientThreadLaunchedIndex;
};

#endif // BOOK_APPLICATION_HPP
