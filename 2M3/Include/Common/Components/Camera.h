#pragma once
#include "Common/Components/Component.h"
#include "Common/Entity.h"

#include <SFML/Graphics/View.hpp>

/**
 * @struct Camera component.
 */
struct Camera : public IdentifiableComponent<Camera>
{
    Camera() = default;

    sf::View playerView;            //!< View of the player.
    std::weak_ptr<Entity> player;   //!< Player on which the camera is attached to.
};