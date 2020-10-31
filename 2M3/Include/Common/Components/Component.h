#pragma once
#include <typeinfo>

struct Component
{};

template <typename T>
struct IdentifiableComponent : Component
{
    static const std::size_t id;
};

template <typename T>
const std::size_t IdentifiableComponent<T>::id = typeid(T).hash_code();