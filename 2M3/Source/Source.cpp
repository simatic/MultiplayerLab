#define _USE_MATH_DEFINES

#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>

int main()
{
	sf::Clock clock;

    sf::RenderWindow window(sf::VideoMode(1600, 900), "SFML works!");
	sf::RectangleShape shape(sf::Vector2f(100, 50));
	shape.setOrigin(sf::Vector2f(50, 25));

	float x = 800;
	float y = 450;
	shape.setPosition(x, y);

	float engineAccel = 0.00001;
	float engineSpeed = 0;
	float engineMaxSpeed = 0.01;

	float steeringAngle = 0;
	float steeringSpeed = M_PI_4;
	float steeringMaxAngle = M_PI * 2 / 3;

	sf::Vector2f carSpeed(0, 0);
	sf::Vector2f carAccel(0, 0);
	float carAngle = 0;
	float maxCarSpeed = 0.05;

	bool braking = false;

    while (window.isOpen())
    {
		sf::Time dt = clock.restart();

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) 
			window.close();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			engineSpeed += engineAccel * dt.asSeconds();
		}
		//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && engineSpeed > 0)
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			braking = true;
			engineSpeed = 0;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))// && engineSpeed <= 0)
		{
			engineSpeed -= engineAccel * dt.asSeconds();
		}
		if (engineSpeed > engineMaxSpeed) engineSpeed = engineMaxSpeed;
		if (engineSpeed < -engineMaxSpeed) engineSpeed = -engineMaxSpeed;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			steeringAngle += steeringSpeed * dt.asSeconds();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			steeringAngle -= steeringSpeed * dt.asSeconds();
		}
		/*if (steeringAngle > steeringMaxAngle) steeringAngle = steeringMaxAngle;
		if (steeringAngle < -steeringMaxAngle) steeringAngle = -steeringMaxAngle;*/

		if (engineSpeed > 0) carSpeed += engineSpeed * sf::Vector2f(cos(carAngle + steeringAngle), -sin(carAngle + steeringAngle));
		else carSpeed += -engineSpeed * sf::Vector2f(-sin(carAngle + steeringAngle), -cos(carAngle + steeringAngle));

		if (braking)
		{
			carSpeed = sf::Vector2f(0, 0);
			braking = false;
		}

		float dx = carSpeed.x;
		float dy = carSpeed.y;
		float length = sqrt(dx * dx + dy * dy);
		if (length > maxCarSpeed)
		{
			carSpeed *= maxCarSpeed / length;
		}
		x += dx;
		y += dy;

		if (x > 1600) x = 0;
		else if (x < 0) x = 1600;

		if (y > 900) y = 0;
		else if (y < 0) y = 900;

		shape.setPosition(x, y);

		if (carSpeed.x != 0 || carSpeed.y != 0)
		{
			carAngle = atan2(carSpeed.y, carSpeed.x);
		}
		shape.setRotation(carAngle * 180.0 / M_PI);

		window.clear();
		window.draw(shape);
		window.display();
    }

    return 0;
}
