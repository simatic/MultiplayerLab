#ifndef BOOK_APPLICATION_HPP
#define BOOK_APPLICATION_HPP

#include "ResourceHolder.h"
#include "ResourceIdentifiers.h"
#include "KeyBinding.h"
#include "StateStack.h"
#include "Client.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>


class Application
{
	public:
		Application();
		void run();
		
		struct ClientInfo
		{
			ClientInfo(Client* client, sf::Thread* thread, /*const sf::Texture& texture,*/ sf::Sprite* sprite);
			std::unique_ptr<Client> client;
			std::unique_ptr<sf::Thread> thread;
			//const sf::Texture* texture;
			std::unique_ptr<sf::Sprite> sprite;
		};

	private:
		void launchClientThread();
		void addClientThread();
		void terminateClientThreads();

		void manageInputs();
		void render();

	private:
		// Main window in which the Clients' views will be rendered.
		sf::RenderWindow		mWindow;

		// Replace mWindow.isOpen() to avoid blocking the access to mWindow from other threads.
		bool _isMainWindowOpen;

		std::vector<std::unique_ptr<ClientInfo>> _clientsInfo;

		// Vector of Clients.
		std::vector<std::shared_ptr<Client>> _clients;

		// Number of Clients in _clients.
		int _clientCount;

		// Thread : _clientThreads[i] corresponds to the Client : _clients[i] 
		std::vector<std::unique_ptr<sf::Thread>> _clientThreads;

		// Common mutex between all the threads to lock variables.
		std::unique_ptr<sf::Mutex> _mutex;

		/* Index of the last launched Thread to keep a track of launched Threads
		 * and not yet launched Threads.
		*/
		int _clientThreadLaunchedIndex;

		// Vector of inputs' type to give to the Clients. The event itself is not needed.
		std::vector <sf::Event> _clientInputs;
};

#endif // BOOK_APPLICATION_HPP
