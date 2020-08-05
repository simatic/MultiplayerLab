#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <SFML/Graphics.hpp>

float length(sf::Vector2f vector);
float scalar(sf::Vector2f u, sf::Vector2f v);
sf::Vector2f rotate(sf::Vector2f v, float a);