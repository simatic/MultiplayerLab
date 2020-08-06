#define _USE_MATH_DEFINES

#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include <World.h>

int main()
{
	sf::Clock clock;

    sf::RenderWindow window(sf::VideoMode(1600, 900), "2M3");

	World world = World();

    while (window.isOpen())
    {
		sf::Time dt = clock.restart();

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) 
			window.close();
		}

		world.update(dt);

		window.clear();
		
		world.draw(window);

		window.display();
    }

    return 0;
}