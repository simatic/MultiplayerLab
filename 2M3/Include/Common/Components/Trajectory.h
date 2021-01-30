#pragma once
#include "Component.h"

#include <SFML/Graphics/VertexArray.hpp>

/**
 * @struct Trajectory component.
 * Holds positions points that are linked together to render a trajectory.
 */
struct Trajectory : public IdentifiableComponent<Trajectory>
{
	Trajectory();

	sf::VertexArray trajectory;	//!< List of points.
};