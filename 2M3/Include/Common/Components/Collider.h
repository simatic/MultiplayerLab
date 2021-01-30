#pragma once
#include "Common/Components/Component.h"
#include "Common/Entity.h"

#include <queue>
#include <SFML/Graphics/RectangleShape.hpp>

/**
 * @struct Collider component.
 * Holds information about the entity collisions with other objects.
 */
struct Collider : public IdentifiableComponent<Collider>
{
	Collider(const sf::Vector2f& size);
	Collider(const sf::RectangleShape& shape);

	bool				collides = false;		//!< If the entity is colliding.
	sf::RectangleShape	shape;					//!< The shape of the collider.
	std::deque<std::weak_ptr<Entity>>	others;	//!< The entities in collision with the entity the component is attached to.
};