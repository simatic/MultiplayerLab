#include "GameState.h"
#include "Common/Managers/ResourceManager.h"
#include "Common/Components/Transform.h"
#include "Common/Components/Kinematics.h"
#include "Common/Components/Collider.h"
#include "Common/Components/CarInput.h"
#include "Common/Components/CarEngine.h"
#include "Common/Components/Health.h"
#include "Common/Components/Gun.h"
#include "Common/Components/Trajectory.h"
#include "Common/Components/Particles.h"
#include "Common/Components/Sprite.h"
#include "Common/Components/RectShape.h"

#include "Common/Systems/BulletSystem.h"
#include "Common/Systems/CarCollisionHandling.h"
#include "Common/Systems/CarDeath.h"
#include "Common/Systems/CarMovementSystem.h"
#include "Common/Systems/CollisionSystem.h"
#include "Common/Systems/GunSystem.h"
#include "Common/Systems/KeyboardInputSystem.h"
#include "Common/Systems/MovementSystem.h"
#include "Common/Systems/ParticleSystem.h"
#include "Common/Systems/HealthBarRenderer.h"
#include "Common/Systems/ParticleRenderer.h"
#include "Common/Systems/SpriteRenderer.h"
#include "Common/Systems/TrajectoryRenderer.h"
#include "Common/Systems/TrajectorySystem.h"
#include "Common/Systems/RectShapeRenderer.h"

#include <iostream>

#include <SFML/Graphics/RenderWindow.hpp>
#include <Common/Components/CameraTarget.h>
#include <Common/Systems/CameraSystem.h>

GameState::GameState(StateStack& stack, Context context) :
	State(stack, context),
	gameManager(GameManager::getInstance())
{
	target = context.window;
	ResourceManager::getInstance()->setTextures(context.textures);
    gameManager->setRenderTarget(target);
    gameManager->setKeyBinding(context.keys);

    Collider    collider = Collider(sf::Vector2f(80, 40));

    sf::FloatRect bounds = collider.shape.getLocalBounds();
	collider.shape.setOrigin(bounds.width / 2, bounds.height / 2);

    sf::Sprite s = sf::Sprite(context.textures->get(Textures::Car));
    s.setScale(sf::Vector2f(0.132f, 0.132f));

    sf::FloatRect spriteBounds = s.getLocalBounds();
	collider.shape.setOrigin(spriteBounds.width / 2, spriteBounds.height / 2);
    s.setOrigin(spriteBounds.width / 2, spriteBounds.height / 2);

	std::shared_ptr<Entity> playerCar = std::make_shared<Entity>(
            Transform(sf::Vector2f(0, 0), 0.f),
            Kinematics(),
            collider,
            CarInput(),
            Health(75, 100),
            HealthBar(sf::Vector2f(100, 10), sf::Color::Red, sf::Color::Green, 0.5f),
            CarEngine(1000, 1000 / 3, 200, 24, 800, 3.1415f / 3, 0.001f, sf::Vector2f(1, 0)),
            Gun(sf::Vector2f(1, 0), sf::seconds(0.1)),
            Trajectory(),
            Particles(sf::Vector2f(0, 0), sf::Color::White, sf::seconds(0.7), sf::seconds(1.0 / 40.0)),
            Sprite(s),
            CameraTarget()
	        );

	playerCar->setLayer(Layer::CarLayer);

	gameManager->addEntity(playerCar);

    createWall(sf::Vector2f(-1, 0));
    createWall(sf::Vector2f(+1, 0));
    createWall(sf::Vector2f(0, -1));
    createWall(sf::Vector2f(0, +1));

    gameManager->addRenderer(std::make_unique<CameraSystem>());
    gameManager->addRenderer(std::make_unique<TrajectoryRenderer>());
    gameManager->addRenderer(std::make_unique<ParticleRenderer>());
    gameManager->addRenderer(std::make_unique<SpriteRenderer>());
    gameManager->addRenderer(std::make_unique<RectShapeRenderer>());
    gameManager->addRenderer(std::make_unique<HealthBarRenderer>());

    gameManager->addSystems<
            KeyboardInputSystem,
            CarMovementSystem,
            CollisionSystem,
            CarCollisionSystem,
            GunSystem,
            BulletSystem,
            TrajectorySystem,
            ParticleSystem,
            CarDeath,
            MovementSystem
    >();

    gameManager->updateSystemLists();
}

void GameState::createWall(sf::Vector2f axis) const {
    const float wallLength = 5000.0f;
    RectShape rect;
    rect.shape = sf::RectangleShape(sf::Vector2f(std::abs(axis.x) * wallLength + std::abs(axis.y) * 10.0f, std::abs(axis.x) * 10.0f + std::abs(axis.y) * wallLength));
    rect.shape.setFillColor(sf::Color::Blue);
    rect.shape.setOrigin(rect.shape.getSize().x / 2, rect.shape.getSize().y / 2);

    std::shared_ptr<Entity> wall = std::make_shared<Entity>(
            Transform(sf::Vector2f(axis.y*wallLength/2.0f, axis.x*wallLength/2.0f), 0.f),
            rect,
            Collider(rect.shape)
    );

    wall->setLayer(Layer::WallLayer);

    gameManager->addEntity(wall);
}

void GameState::draw()
{
    gameManager->render(sf::seconds(0));
}

bool GameState::update(sf::Time dt)
{
	gameManager->update(dt);
	return true;
}

bool GameState::handleEvent(const sf::Event& event)
{
	return true;
}

