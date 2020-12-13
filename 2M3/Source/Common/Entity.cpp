#include "Common/Entity.h"

Entity::Entity(const Entity& entity)
{
    for (auto&& [id, component]: entity.components)
    {
        components.emplace(id, std::make_unique<Component>(*component));
    }
}

/**
 * @param id Entity's id.
 */
void Entity::setID(const std::uint32_t id)
{
    this->id = id;
}

/**
 * @return Entity's id.
 */
std::uint32_t Entity::getID() const
{
    return id;
}

/**
 * @param layer Entity's layer
 */
void Entity::setLayer(const Layer layer)
{
    this->layer = layer;
}

/**
 * @return Entity's layer
 */
Layer Entity::getLayer() const
{
    return layer;
}

/**
 * @return Entity's signature
 */
const Signature& Entity::getSignature() const
{
    return signature;
}