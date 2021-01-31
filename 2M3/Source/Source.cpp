#define _USE_MATH_DEFINES
#define PROFILING_IMPLEMENTATION
#include "Profiling.h"

#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include <Application/Application.h>

int main()
{
    std::ios::sync_with_stdio(false);
	Application application;
	application.run();
	//sf::Clock clock;

 //   sf::RenderWindow window(sf::VideoMode(1600, 900), "2M3");

	//World world = World();

 //   while (window.isOpen())
 //   {
	//	sf::Time dt = clock.restart();

	//	sf::Event event;
	//	while (window.pollEvent(event))
	//	{
	//		if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) 
	//		window.close();
	//	}

	//	world.update(dt);

	//	window.clear();
	//	
	//	world.draw(window);

	//	window.display();
 //   }

    return 0;
}