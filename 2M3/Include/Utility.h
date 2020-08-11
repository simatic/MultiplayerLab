#ifndef BOOK_UTILITY_HPP
#define BOOK_UTILITY_HPP

#define _USE_MATH_DEFINES
#include <math.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/System/Vector2.hpp>

#include <sstream>


namespace sf
{
	class Sprite;
	class Text;
}

class Animation;

// Since std::to_string doesn't work on MinGW we have to implement
// our own to support all platforms.
template <typename T>
std::string		toString(const T& value);

// Convert enumerators to strings
std::string		toString(sf::Keyboard::Key key);

// Call setOrigin() with the center of the object
void			centerOrigin(sf::Sprite& sprite);
void			centerOrigin(sf::Text& text);
//void			centerOrigin(Animation& animation);

float length(sf::Vector2f vector);
float dotProduct(sf::Vector2f u, sf::Vector2f v);
sf::Vector2f rotate(sf::Vector2f v, float a);
float toRadians(float a);
float toDegrees(float a);
sf::Vector2f unitVector(sf::Vector2f v);

#include "Utility.inl"
#endif // BOOK_UTILITY_HPP
