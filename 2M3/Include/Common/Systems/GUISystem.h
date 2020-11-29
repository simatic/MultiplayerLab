#pragma once
#include "Common/Systems/System.h"
#include "Common/Components/GUI.h"
#include "OldEntity.h"

class GUISystem : public SignedSystem<GUI>
{
public:
	static void update(const sf::Time dt, OldEntity* entity);
};