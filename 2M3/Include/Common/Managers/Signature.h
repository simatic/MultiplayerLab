#pragma once
#include <iostream>
#include <unordered_map>
#include <bitset>
#include <iostream>
#define MAX_COMPONENTS 32

class Signature
{
public:
	Signature();

	template <typename First, typename... Components>
	void addComponent();
	
	template <typename First, typename... Components>
	void removeComponent();

	friend std::ostream& operator<<(std::ostream& os, const Signature& signature);

private:
	template <typename Component>
	static bool isComponentRegistered();

	template <typename Component>
	static void registerComponent();

	template <typename Component>
	void _addComponent();
	
	template <typename Component>
	void _removeComponent();

	static std::size_t registeredComponentsCount;
	static std::unordered_map<std::size_t, std::size_t> idToBitIndexMap;

	std::bitset<MAX_COMPONENTS> signature;
};

template <typename Component>
bool Signature::isComponentRegistered()
{
	return Signature::idToBitIndexMap.find(Component::id) == Signature::idToBitIndexMap.end();
}

template <typename Component>
void Signature::registerComponent()
{
	Signature::idToBitIndexMap[Component::id] = Signature::registeredComponentsCount++;
}

template <typename First, typename... Components>
void Signature::addComponent()
{
	_addComponent<First>();

	if (sizeof...(Components) > 0)
	{
		((_addComponent<Components>()), ...);
	}
}

template <typename Component>
void Signature::_addComponent()
{
	if (Signature::isComponentRegistered<Component>())
	{
		Signature::registerComponent<Component>();
	} 

	signature[idToBitIndexMap[Component::id]] = true;
}

template <typename First, typename... Components>
void Signature::removeComponent()
{
	_removeComponent<First>();

	if (sizeof...(Components) > 0)
	{
		((_removeComponent<Components>()), ...);
	}
}

template <typename Component>
void Signature::_removeComponent()
{
	if (Signature::isComponentRegistered<Component>())
	{
		Signature::registerComponent<Component>();
	}

	signature[idToBitIndexMap[Component::id]] = false;
}