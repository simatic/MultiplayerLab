#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <SFML/Graphics.hpp>

float length(sf::Vector2f vector);
float dotProduct(sf::Vector2f u, sf::Vector2f v);
sf::Vector2f rotate(sf::Vector2f v, float a);
float toRadians(float a);
float toDegrees(float a);
sf::Vector2f unitVector(sf::Vector2f v);