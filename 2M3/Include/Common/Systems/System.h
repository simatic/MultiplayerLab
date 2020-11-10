#pragma once
#include "Common/Managers/Signature.h"

class System
{};

template <typename... Components>
class SignedSystem : public System
{
public:
	static Signature signature;
};

template <typename... Components>
Signature SignedSystem<Components...>::signature = Signature();