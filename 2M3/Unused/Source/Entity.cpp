#include "Common/Entity.h"

Entity* Entity::instanciate()
{
    GameManager::getInstance()->addEntity(std::unique_ptr<Entity>(new Entity(GameManager::getInstance())));
}

Entity::Entity(GameManager* gameManager) :
    id(0),
    signature(),
    components(),
    gameManager(gameManager)
{}

void Entity::setId(const std::uint32_t id)
{
    this->id = id;
}

std::uint32_t Entity::getId() const
{
    return id;
}


void Entity::setActive(bool isActive)
{
    
}