#pragma once
#include <typeinfo>
#include <iostream>

/**
 * Abstract struct for Component.
 */
struct Component
{
public:
    friend std::ostream& operator<<(std::ostream& os, const Component& component) { return component.serialize(os); };
    friend std::istream& operator>>(std::istream& is, Component& component) { return component.deserialize(is); };

protected:
    virtual std::ostream& serialize(std::ostream& os) const { return os; };
    virtual std::istream& deserialize(std::istream& is) { return is; };
};

/**
 * Helper struct to generate a compile-time id for a Component.
 */
template <typename T>
struct IdentifiableComponent : Component
{
    static const std::size_t id;
};

template <typename T>
const std::size_t IdentifiableComponent<T>::id = typeid(T).hash_code();