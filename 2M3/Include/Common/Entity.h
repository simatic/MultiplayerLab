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
	WallLayer,
	DefaultLayer
};

class Entity: public std::enable_shared_from_this<Entity>
{
public:
	Entity(const Entity& entity);

	/// Constructor to initialize an entity with all its components at once and default layer.
    template<typename... Components>
    explicit Entity(Components... components);

	/// Constructor to initialize an entity with all its components and a custom layer.
	template<typename... Components>
	explicit Entity(const Layer layer, Components... components);

	void 				setID(const std::uint32_t id);
	std::uint32_t 		getID() const;

	void 				setLayer(const Layer layer);
	Layer 				getLayer() const;

	const Signature& 	getSignature() const;

	template <typename T>
	void 				addComponent(T& component);

	template <typename T>
	T* 					getComponent();

	template <typename T>
	void 				removeComponent();

private:
	std::uint32_t id = 0;
	Layer layer = Layer::DefaultLayer;
	Signature signature{};
	std::unordered_map<std::size_t, std::unique_ptr<Component>> components{};
};

template<typename... Component>
Entity::Entity(Component... component) {
    (
            (
                    signature.addComponent<Component>(),
                    components[Component::id] = std::make_unique<Component>(component)
            )
    , ...);
}

template<typename... Component>
Entity::Entity(const Layer layer, Component... component) : 
	Entity(component...) {
	this->layer = layer;
}

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