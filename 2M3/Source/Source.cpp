#define _USE_MATH_DEFINES

#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include "World.h"
#include "Application.h"
#include "Common/Managers/Signature.h"

int main()
{
	Signature sig;
	sig.addComponent<Transform, Bullet>();
	sig.removeComponent<Transform>();

	std::cout << sig << std::endl;

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