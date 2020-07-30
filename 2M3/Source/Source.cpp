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

int main()
{
	sf::Clock clock;

    sf::RenderWindow window(sf::VideoMode(1600, 900), "SFML works!");
	sf::RectangleShape shape(sf::Vector2f(100, 50));
	shape.setOrigin(sf::Vector2f(50, 25));

	float x = 800;
	float y = 450;
	shape.setPosition(x, y);

	sf::Vector2f carSpeed(0, 0);
	float carMaxSpeed = 500;
	float carAngle = 0;

	float engineSpeed = 0;
	float engineMaxSpeed = 20;
	float engineAccel = 5;

	float steeringAngle = M_PI / 100;

	bool forward = true;

	sf::VertexArray lines(sf::Lines, 4);
	lines[0].color = sf::Color::Red;
	lines[1].color = sf::Color::Red;
	lines[2].color = sf::Color::Green;
	lines[3].color = sf::Color::Green;

    while (window.isOpen())
    {
		sf::Time dt = clock.restart();

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) 
			window.close();
		}

		forward = scalar(carSpeed, sf::Vector2f(cos(carAngle), -sin(carAngle))) >= 0;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			float factor = 1;
			if (!forward) factor = 10;

			engineSpeed += factor * engineAccel * dt.asSeconds();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			float factor = 1;
			if (forward) factor = 10;

			engineSpeed -= factor * engineAccel * dt.asSeconds();
		}
		if (engineSpeed > engineMaxSpeed) engineSpeed = engineMaxSpeed;
		if (engineSpeed < -engineMaxSpeed) engineSpeed = -engineMaxSpeed;

		float angle = 0;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			angle += steeringAngle;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			angle -= steeringAngle;
		}

		sf::Vector2f direction(cos(angle + carAngle), -sin(angle + carAngle));
		//if (!forward) direction = -sf::Vector2f(cos(angle + M_PI - carAngle), -sin(angle + M_PI - carAngle));

		carSpeed += engineSpeed * direction;
		float l = length(carSpeed);
		if (l > carMaxSpeed)
		{
			carSpeed *= carMaxSpeed / l;
		}
		
		x += carSpeed.x * dt.asSeconds();
		y += carSpeed.y * dt.asSeconds();
		if (carSpeed.x != 0) carAngle = -atan2(carSpeed.y, carSpeed.x);
		if (!forward) carAngle += M_PI;

		if (x > 1600) x = 0;
		else if (x < 0) x = 1600;

		if (y > 900) y = 0;
		else if (y < 0) y = 900;

		shape.setPosition(x, y);
		shape.setRotation(-carAngle * 180.0 / M_PI);

		lines[0].position = sf::Vector2f(x, y);
		lines[1].position = sf::Vector2f(x, y) + (float)100 * sf::Vector2f(cos(carAngle), -sin(carAngle));
		lines[2].position = sf::Vector2f(x, y);
		lines[3].position = sf::Vector2f(x, y) + (float)100 * carSpeed;

		window.clear();
		window.draw(shape);
		window.draw(lines);
		window.display();
    }

    return 0;
}