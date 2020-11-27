#pragma once
#include <SFML/System/Time.hpp>
#include "Common/Systems/System.h"
#include "OldEntity.h"
#include "Common/Components/Kinematics.h"
#include "Common/Components/Bullet.h"

class BulletSystem : public SignedSystem<Transform, Kinematics, Bullet>
{
public:
	static void update(const sf::Time& dt, OldEntity* entity, std::vector<OldEntity*>& others, std::set<OldEntity::Pair>& pairs);
};