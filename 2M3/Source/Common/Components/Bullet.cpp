#include "Common/Components/Bullet.h"

Bullet::Bullet(const unsigned int damage, const float maxSpeed, const sf::Time& lifetime, const bool guided, OldEntity* owner, OldEntity* target, const float detectionRange, const float guideRate) :
	damage(damage),
	maxSpeed(maxSpeed),
	lifetime(lifetime),
	guided(guided),
	owner(owner),
	target(target),
	detectionRange(detectionRange),
	guideRate(guideRate)
{}