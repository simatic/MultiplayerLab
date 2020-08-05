#define _USE_MATH_DEFINES

#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include <Utility.h>
#include <World.h>

//float length(sf::Vector2f vector)
//{
//	return sqrt(vector.x * vector.x + vector.y * vector.y);
//}
//
//float scalar(sf::Vector2f u, sf::Vector2f v)
//{
//	return u.x * v.x + u.y * v.y;
//}
//
//sf::Vector2f rotate(sf::Vector2f v, float a)
//{
//	if (v.x == 0 && v.y == 0) return sf::Vector2f(0, 0);
//	float l = length(v);
//	if (v.x == 0)
//	{
//		float prevA = M_PI_2 * v.y / abs(v.y);
//		return l * sf::Vector2f(cos(prevA + a), -sin(prevA + a));
//	}
//
//	float prevA = -atan2(v.y, v.x);
//	return l * sf::Vector2f(cos(prevA + a), -sin(prevA + a));
//}

int main()
{
	/*Application app;
	app.run();*/
	
	sf::Clock clock;

    sf::RenderWindow window(sf::VideoMode(1600, 900), "SFML works!");
	/*sf::RectangleShape shape(sf::Vector2f(40, 20));
	shape.setOrigin(sf::Vector2f(20, 10));

	sf::Vector2f carPos(800, 450);
	shape.setPosition(carPos);
	sf::Vector2f carDirection(1, 0);
	sf::Vector2f carSpeed(0, 0);
	float carMaxSpeed = 1000;
	float backCarMaxSpeed = carMaxSpeed / 3;
	float carAngle = 0;
	float carAccel = 200;

	float R = 12;

	bool forward = true;
	bool drifting = false;
	float prevDriftingSign = 0;
	float driftThreshold = carMaxSpeed - 200;
	float driftAngle = M_PI / 3;

	float drag = 0.001;

	sf::VertexArray tires(sf::Lines, 1);
	tires[0].position = carPos;*/

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

		//float l = length(carSpeed);
		//float accel = 0;
		//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		//{
		//	float f = 1;
		//	if (!forward) f = 10;
		//	accel += f * carAccel;
		//}
		//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		//{
		//	float f = 1;
		//	if (forward) f = 10;
		//	accel -= f * carAccel;
		//}
		//if (accel == 0 && l > 200)
		//{
		//	accel = (l * l + 2 * l) * drag;
		//	if (forward) accel *= -1;
		//}
		//else if (accel == 0)
		//{
		//	carSpeed = sf::Vector2f(0, 0);
		//}

		//float angle = 0;
		//float angleSign = 0;
		//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && l > 50)
		//{
		//	angle += M_PI / 3;
		//	angleSign += 1;
		//}
		//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && l > 50)
		//{
		//	angle -= M_PI / 3;
		//	angleSign -= 1;
		//}

		//float tangAccel = accel * cos(angle);
		//float radAccel = accel * sin(angle);

		//sf::Vector2f tangAccelVector = tangAccel * carDirection;
		//carSpeed += tangAccelVector * dt.asSeconds();
		//l = length(carSpeed);
		//if (forward && l > carMaxSpeed)
		//{
		//	carSpeed *= carMaxSpeed / l;
		//}
		//else if (!forward && l > backCarMaxSpeed)
		//{
		//	carSpeed *= backCarMaxSpeed / l;
		//}

		//bool prevDrifting = drifting;
		//drifting = forward && l > driftThreshold && angleSign != 0;

		//float theta = sqrt(abs(radAccel) / R) * dt.asSeconds();
		//forward = scalar(carSpeed, carDirection) >= 0;
		//if (!forward)
		//{
		//	carSpeed = rotate(carSpeed, -theta * angleSign);
		//	carDirection = rotate(carDirection, -theta * angleSign);
		//}
		//else
		//{
		//	carSpeed = rotate(carSpeed, theta * angleSign);
		//	carDirection = rotate(carDirection, theta * angleSign);
		//}

		//if (prevDrifting && !drifting)
		//{
		//	carDirection = rotate(carDirection, prevDriftingSign * driftAngle);
		//	carSpeed = rotate(carSpeed, prevDriftingSign * driftAngle);
		//}
		//prevDriftingSign = angleSign;

		//carPos += carSpeed * dt.asSeconds();

		///*if (carPos.x > 1600) carPos.x = 0;
		//else if (carPos.x < 0) carPos.x = 1600;

		//if (carPos.y > 900) carPos.y = 0;
		//else if (carPos.y < 0) carPos.y = 900;

		//shape.setPosition(carPos);*/

		//carAngle = 0;
		//if (carDirection.x != 0) carAngle = -atan2(carDirection.y, carDirection.x);
		//if (carDirection.x == 0 && carDirection.y != 0) carAngle = M_PI_2 * carDirection.y / abs(carDirection.y);
		//if (drifting) carAngle += angleSign * driftAngle;
		//shape.setRotation(-carAngle * 180.0 / M_PI);

		//for (int i = 0; i < tires.getVertexCount(); i++)
		//{
		//	tires[i].position -= carSpeed * dt.asSeconds();
		//}
		//tires.append(sf::Vertex(shape.getPosition() - (float)20 * carDirection));
		//tires.append(sf::Vertex(shape.getPosition() - (float)20 * carDirection));

		world.update(dt);

		window.clear();
		world.draw(window);
		/*window.draw(shape);
		window.draw(tires);*/
		window.display();
    }

    return 0;
}