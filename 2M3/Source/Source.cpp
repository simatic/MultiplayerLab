#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{
	sf::Clock clock;

    sf::RenderWindow window(sf::VideoMode(1600, 900), "SFML works!");
	sf::RectangleShape shape(sf::Vector2f(50, 100));
	shape.setOrigin(sf::Vector2f(25, 50));

	float x = 800;
	float y = 450;
	shape.setPosition(x, y);

	float speed = 0;
	float angle = 0;


    while (window.isOpen())
    {
		sf::Time dt = clock.restart();

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) 
			window.close();
		}
		
		speed *= 0.9 * dt.asSeconds();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			speed += 0.5 * dt.asSeconds() * 100;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			speed -= 0.5 * dt.asSeconds() * 100;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			angle += speed / 2;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			angle -= speed / 2;
		}

		x -= speed * sin(angle);
		y -= speed * cos(angle);

		shape.setPosition(x, y);
		shape.setRotation(angle);

		window.clear();
		window.draw(shape);
		window.display();
    }

    return 0;
}
