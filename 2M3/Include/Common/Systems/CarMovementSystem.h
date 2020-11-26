#pragma once
#include <SFML/System/Time.hpp>
#include "Common/Systems/System.h"
#include "Common/Components/Kinematics.h"
#include "Common/Components/CarInput.h"
#include "Common/Components/CarEngine.h"
#include "OldEntity.h"
#include "CarLogic.h"

class CarMovementSystem : public SignedSystem<Transform, Kinematics, CarInput, CarEngine>
{
public:
	static void update(const sf::Time dt, CarLogic* entity, std::vector<OldEntity*>& newEntities);
};