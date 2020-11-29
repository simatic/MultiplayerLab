#pragma once
#include "Common/Components/Component.h"
#include "Managers/Signature.h"

#include <cstdint>
#include <unordered_map>
#include <memory>

enum class Layer
{
	CarLayer,
	ProjectileLayer,
	DefaultLayer
};

class Entity
{
public:
    Entity() = default;
	Entity(const Entity& entity);

	std::size_t size() const { return components.size(); } 

	void setID(const std::uint32_t id) { this->id = id; }
	std::uint32_t getID() const { return id; }

	void setLayer(const Layer layer) { this->layer = layer; }
	Layer getLayer() const { return layer; }

	const Signature& getSignature() const { return signature; }

	template <typename T>
	void addComponent(T& component);

	template <typename T>
	T* getComponent();

	template <typename T>
	void removeComponent();

private:
	std::uint32_t id;
	Layer layer;
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
	signature.removeComponent<T>();
	components.erase(T::id);
}