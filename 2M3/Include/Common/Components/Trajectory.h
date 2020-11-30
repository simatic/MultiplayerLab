#pragma once
#include "Component.h"

#include <SFML/Graphics/VertexArray.hpp>

struct Trajectory : public IdentifiableComponent<Trajectory>
{
	Trajectory();

	sf::VertexArray trajectory;
};