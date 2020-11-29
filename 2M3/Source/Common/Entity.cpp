#include "Common/Entity.h"

Entity::Entity(const Entity& entity)
{
    for (auto&& [id, component]: entity.components)
    {
        components.emplace(id, std::make_unique<Component>(*component));
    }
}