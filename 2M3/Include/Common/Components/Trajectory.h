#pragma once
#include <SFML/Graphics/VertexArray.hpp>
#include "Component.h"

struct Trajectory : public IdentifiableComponent<Trajectory>
{
	sf::VertexArray trajectory;

	Trajectory();
};