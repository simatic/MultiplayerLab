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
	
	Transform   t = Transform(sf::Vector2f(0, 0), 0.f);
    Kinematics  k = Kinematics();
    Collider    c = Collider(sf::Vector2f(80, 40));
    CarInput    inputs = CarInput();
    CarEngine   engine = CarEngine(1000, 1000 / 3, 200, 24, 800, 3.1415f / 3, 0.001f, sf::Vector2f(1, 0));
    Health      health = Health(100, 100);
    Gun         gun = Gun(sf::Vector2f(1, 0), sf::seconds(0.1));
    Trajectory  trajectory = Trajectory();
    Particles   particles = Particles(sf::Vector2f(0, 0), sf::Color::White, sf::seconds(0.7), sf::seconds(1.0 / 40.0));
    CameraTarget cameraTarget = CameraTarget();

    trajectory.trajectory[0].position = t.position;
    
    sf::FloatRect bounds = c.shape.getLocalBounds();
	c.shape.setOrigin(bounds.width / 2, bounds.height / 2);

    sf::Sprite s = sf::Sprite(context.textures->get(Textures::Car));
    s.setScale(sf::Vector2f(0.132f, 0.132f));

    sf::FloatRect spriteBounds = s.getLocalBounds();
	c.shape.setOrigin(spriteBounds.width / 2, spriteBounds.height / 2);
    s.setOrigin(spriteBounds.width / 2, spriteBounds.height / 2);

    Sprite sprite = Sprite(s);

	std::shared_ptr<Entity> e = std::make_shared<Entity>();
    e->addComponent<Transform>(t);
    e->addComponent<Kinematics>(k);
    e->addComponent<Collider>(c);
    e->addComponent<CarInput>(inputs);
    e->addComponent<Health>(health);
    e->addComponent<CarEngine>(engine);
    e->addComponent<Gun>(gun);
    e->addComponent<Trajectory>(trajectory);
    e->addComponent<Particles>(particles);
    e->addComponent<Sprite>(sprite);
    e->addComponent<CameraTarget>(cameraTarget);

	gameManager->addEntity(e);

    std::shared_ptr<Entity> wall = std::make_shared<Entity>();
    RectShape rect;
    rect.shape = sf::RectangleShape(sf::Vector2f(10.0f, 1000.0f));
    rect.shape.setFillColor(sf::Color::Blue);
    Transform trans = Transform(sf::Vector2f(-1000, 0), 0.f);
    wall->addComponent<Transform>(trans);
    wall->addComponent<RectShape>(rect);

    gameManager->addEntity(wall);

    std::unique_ptr<System> bs = std::make_unique<BulletSystem>();
    std::unique_ptr<System> cch = std::make_unique<CarCollisionSystem>();
    std::unique_ptr<System> cd = std::make_unique<CarDeath>();
    std::unique_ptr<System> cms = std::make_unique<CarMovementSystem>();
    std::unique_ptr<System> cs = std::make_unique<CollisionSystem>();
    std::unique_ptr<System> gs = std::make_unique<GunSystem>();
    std::unique_ptr<System> kis = std::make_unique<KeyboardInputSystem>();
    std::unique_ptr<System> ms = std::make_unique<MovementSystem>();
    std::unique_ptr<System> ps = std::make_unique<ParticleSystem>();
    std::unique_ptr<System> ts = std::make_unique<TrajectorySystem>();
    std::unique_ptr<System> rs = std::make_unique<SpriteRenderer>();
    std::unique_ptr<System> rts = std::make_unique<TrajectoryRenderer>();
    std::unique_ptr<System> rps = std::make_unique<ParticleRenderer>();
    std::unique_ptr<System> rsr = std::make_unique<RectShapeRenderer>();
    std::unique_ptr<System> cameraSystem = std::make_unique<CameraSystem>();

    gameManager->addSystem(std::move(kis));
    gameManager->addSystem(std::move(cms));
    gameManager->addSystem(std::move(cs));
    gameManager->addSystem(std::move(cch));
    gameManager->addSystem(std::move(gs));
    gameManager->addSystem(std::move(bs));
    gameManager->addSystem(std::move(ts));
    gameManager->addSystem(std::move(ps));
    gameManager->addSystem(std::move(cd));
    gameManager->addSystem(std::move(ms));
    gameManager->addSystem(std::move(cameraSystem));
    gameManager->addRenderer(std::move(rts));
    gameManager->addRenderer(std::move(rps));
    gameManager->addRenderer(std::move(rs));
    gameManager->addRenderer(std::move(rsr));

    gameManager->updateSystemLists();
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

