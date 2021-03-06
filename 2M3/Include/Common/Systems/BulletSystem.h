#pragma once
#include "Common/Systems/System.h"
#include "Common/Components/Transform.h"
#include "Common/Components/Kinematics.h"
#include "Common/Components/Bullet.h"

/**
 * @class BulletSystem
 * Handles bullets movement and lifetime.
 */
class BulletSystem : public LogicSystem<Transform, Kinematics, Bullet>
{
public:
	explicit BulletSystem(GameManager* const gameManager);

	void update(const sf::Time& dt) override;
};