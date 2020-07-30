#define _USE_MATH_DEFINES

#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>

float length(sf::Vector2f vector)
{
	return sqrt(vector.x * vector.x + vector.y * vector.y);
}

float scalar(sf::Vector2f u, sf::Vector2f v)
{
	return u.x * v.x + u.y * v.y;
}

sf::Vector2f rotate(sf::Vector2f v, float a)
{
	if (v.x == 0 && v.y == 0) return sf::Vector2f(0, 0);
	float l = length(v);
	if (v.x == 0)
	{
		float prevA = M_PI_2 * v.y / abs(v.y);
		return l * sf::Vector2f(cos(prevA + a), -sin(prevA + a));
	}

	float prevA = -atan2(v.y, v.x);
	return l * sf::Vector2f(cos(prevA + a), -sin(prevA + a));
}

int main()
{
	sf::Clock clock;

    sf::RenderWindow window(sf::VideoMode(1600, 900), "SFML works!");
	sf::RectangleShape shape(sf::Vector2f(100, 50));
	shape.setOrigin(sf::Vector2f(50, 25));

	sf::Vector2f carPos(800, 450);
	shape.setPosition(carPos);
	sf::Vector2f carDirection(1, 0);
	sf::Vector2f carSpeed(0, 0);
	float carAngle = 0;

	float R = 50;

    while (window.isOpen())
    {
		sf::Time dt = clock.restart();

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) 
			window.close();
		}

		float accel = 0;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			accel += 100;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			accel -= 100;
		}

		float angle = 0;
		float angleSign = 0;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			angle += M_PI / 3;
			angleSign += 1;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			angle -= M_PI / 3;
			angleSign -= 1;
		}

		float tangAccel = accel * cos(angle);
		float radAccel = accel * sin(angle);

		sf::Vector2f tangAccelVector = tangAccel * carDirection;
		carSpeed += tangAccelVector * dt.asSeconds();

		float theta = sqrt(abs(radAccel) / R) * dt.asSeconds();

		if (scalar(carSpeed, carDirection) < 0)
		{
			carSpeed = rotate(carSpeed, -theta * angleSign);
			carDirection = rotate(carDirection, -theta * angleSign);
		}
		else
		{
			carSpeed = rotate(carSpeed, theta * angleSign);
			carDirection = rotate(carDirection, theta * angleSign);
		}

		carPos += carSpeed * dt.asSeconds();

		if (carPos.x > 1600) carPos.x = 0;
		else if (carPos.x < 0) carPos.x = 1600;

		if (carPos.y > 900) carPos.y = 0;
		else if (carPos.y < 0) carPos.y = 900;

		shape.setPosition(carPos);

		carAngle = 0;
		if (carDirection.x != 0) carAngle = -atan2(carDirection.y, carDirection.x);
		if (carDirection.x == 0 && carDirection.y != 0) carAngle = M_PI_2 * carDirection.y / abs(carDirection.y);

		shape.setRotation(-carAngle * 180.0 / M_PI);

		window.clear();
		window.draw(shape);
		window.display();
    }

    return 0;
}