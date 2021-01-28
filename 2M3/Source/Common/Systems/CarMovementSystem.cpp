#include "Common/Systems/CarMovementSystem.h"
#include "Utility.h"

CarMovementSystem::CarMovementSystem(GameManager* const gameManager) :
	LogicSystem<Transform, Kinematics, CarInput, VehicleEngine>(gameManager)
{}

void CarMovementSystem::update(const sf::Time& dt)
{
	for (Entity* entity: entities)
	{	
		Transform* transform = entity->getComponent<Transform>();
		Kinematics* kinematics = entity->getComponent<Kinematics>();
		CarInput* inputs = entity->getComponent<CarInput>();
		VehicleEngine* engine = entity->getComponent<VehicleEngine>();
		
		float l = length(kinematics->velocity);

		float angle = 0;
		float angleSign = 0;
		if (inputs->left && l > 50)
		{
			angle += M_PI / 3;
			angleSign += 1;
		}
		if (inputs->right && l > 50)
		{
			angle -= M_PI / 3;
			angleSign -= 1;
		}

		float accel = 0;
		bool driftBrake = false;
		if (inputs->up)
		{
			float f = 1;
			if (!engine->isGoingForward) f = 10;
			accel += f * engine->acceleration;
		}
		if (inputs->down)
		{
			if (engine->isGoingForward && l > engine->driftThreshold && angleSign != 0) driftBrake = true;
			else
			{
				float f = 1;
				if (engine->isGoingForward) f = 10;
				accel -= f * engine->acceleration;
			}
		}
		if (accel == 0 && l > 200)
		{
			accel = (l * l + 2 * l) * engine->drag;
			if (engine->isGoingForward) accel *= -1;
		}
		else if (accel == 0)
		{
			kinematics->velocity = sf::Vector2f(0, 0);
		}

		float tangAccel = accel * cos(angle);
		float radAccel = accel * sin(angle);
		sf::Vector2f tangAccelVector = tangAccel * engine->direction;
		kinematics->velocity += tangAccelVector * dt.asSeconds();
		l = length(kinematics->velocity);
		if (engine->isGoingForward && l > engine->maxSpeed)
		{
			kinematics->velocity *= engine->maxSpeed / l;
		}
		else if (!engine->isGoingForward && l > engine->backwardsMaxSpeed)
		{
			kinematics->velocity *= engine->backwardsMaxSpeed / l;
		}

		bool prevDrifting = engine->isDrifting;
		engine->isDrifting = engine->isGoingForward && l > engine->driftThreshold && angleSign != 0 && driftBrake;

		float theta = sqrt(abs(radAccel) / engine->turnRadius) * dt.asSeconds();
		engine->isGoingForward = dotProduct(kinematics->velocity, engine->direction) >= 0;
		if (!engine->isGoingForward)
		{
			kinematics->velocity = rotate(kinematics->velocity, -theta * angleSign);
			engine->direction = rotate(engine->direction, -theta * angleSign);
		}
		else
		{
			kinematics->velocity = rotate(kinematics->velocity, theta * angleSign);
			engine->direction = rotate(engine->direction, theta * angleSign);
		}

		if (prevDrifting && !engine->isDrifting)
		{
			engine->direction = rotate(engine->direction, engine->previousDriftingSign * engine->driftThreshold);
			kinematics->velocity = rotate(kinematics->velocity, engine->previousDriftingSign * engine->driftAngle);
		}
		engine->previousDriftingSign = angleSign;

		float carAngle = 0;
		if (engine->direction.x != 0) carAngle = -atan2(engine->direction.y, engine->direction.x);
		if (engine->direction.x == 0 && engine->direction.y != 0) carAngle = M_PI_2 * engine->direction.y / abs(engine->direction.y);
		if (engine->isDrifting) carAngle += angleSign * engine->driftAngle;
		transform->rotation = -carAngle * 180.0 / M_PI;
	}
}