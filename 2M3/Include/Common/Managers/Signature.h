#pragma once
#include <iostream>
#include <unordered_map>
#include <bitset>
#include <iostream>
#define MAX_COMPONENTS 32

/**
 * @class Signature
 * A signature is used to track components of the object it is attached to.
 * An object that uses Components may use a Signature to keep track of the Components in its possession.
 */
class Signature
{
public:
	Signature();

	template <typename... Components>
	static Signature 	 generate();

	template <typename First, typename... Components>
	void 				 addComponent();
	
	template <typename First, typename... Components>
	void 				 removeComponent();

	friend std::ostream& operator<<(std::ostream& os, const Signature& signature);
	Signature 			 operator&(const Signature& signature) const;
	bool 				 operator==(const Signature& signature) const;

private:
	template <typename Component>
	static bool 		 isComponentRegistered();

	template <typename Component>
	static void  		 registerComponent();

	template <typename Component>
	void 				 _addComponent();
	
	template <typename Component>
	void 				 _removeComponent();

	static thread_local std::size_t registeredComponentsCount;
	static thread_local std::unordered_map<std::size_t, std::size_t> idToBitIndexMap;

	std::bitset<MAX_COMPONENTS> bitset;
};

/**
 * Generate a signature with the given Components.
 * @tparam Components The list of components to generate the signature.
 * @return The generated signature.
 */
template <typename... Components>
Signature Signature::generate()
{
	Signature signature;
	
	if (sizeof...(Components) > 0)
	{
		signature.addComponent<Components...>();
	}

	return signature;
}

/**
 * @tparam Component The given component
 * @return true if the component is registered in the idToBitIndexMap.
 */
template <typename Component>
bool Signature::isComponentRegistered()
{
	return Signature::idToBitIndexMap.find(Component::id) != Signature::idToBitIndexMap.end();
}

/**
 * Register the given component in the idToBitIndexMap.
 * @tparam Component The given component.
 */
template <typename Component>
void Signature::registerComponent()
{
	Signature::idToBitIndexMap[Component::id] = Signature::registeredComponentsCount++;
}

/**
 * Update signature to reflect newly added components.
 * @tparam Components The newly added components.
 */
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
	if (!Signature::isComponentRegistered<Component>())
	{
		Signature::registerComponent<Component>();
	} 

	bitset[idToBitIndexMap[Component::id]] = true;
}

/**
 * Update signature to reflect removed components.
 * @tparam Components The removed components.
 */
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
	if (!Signature::isComponentRegistered<Component>())
	{
		Signature::registerComponent<Component>();
	}

	bitset[idToBitIndexMap[Component::id]] = false;
}