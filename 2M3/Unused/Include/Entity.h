#pragma once
#include <unordered_map>
#include <memory>
#include "Common/Managers/GameManager.h"
#include "Common/Managers/Signature.h"
#include "Common/Components/Component.h"

class GameManager;

class Entity
{
public:
	static Entity* instanciate();

	void setId(const std::uint32_t id);
	std::uint32_t getId() const;

	void setActive(const bool isActive);
	bool isActive() const;

    template <typename T>
	void addComponent(T& component);

	template <typename T>
	T* getComponent();

	template <typename T>
	void removeComponent();

private:
	Entity(GameManager* gameManager);

	template <typename T>
	void _addComponent(T& component);

	template <typename T>
	void _removeComponent();

private:
	bool active = false;
	GameManager* gameManager;
	std::uint32_t id;
    Signature signature;
    std::unordered_map<std::size_t, std::unique_ptr<Component>> components;
};

template <typename T>
void Entity::addComponent(T& component)
{
    signature.addComponent<T>();
	components[T::id] = std::make_unique<T>(component);
}

template <typename T>
T* Entity::getComponent()
{
	return static_cast<T*>(components[T::id].get());
}

template <typename T>
void Entity::removeComponent()
{
	components.erase(T::id);
    signature.removeComponent<T>();
}

template <typename T>
void Entity::_addComponent(T& component)
{
    signature.addComponent<T>();
	components[T::id] = std::make_unique<T>(component);
}

template <typename T>
void Entity::_removeComponent()
{
	components.erase(T::id);
    signature.removeComponent<T>();
}