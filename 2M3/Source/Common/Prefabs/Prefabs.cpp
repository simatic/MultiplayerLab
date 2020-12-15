#include "Common/Prefabs/Prefabs.h"

#include "Common/Managers/ResourceManager.h"

#include "Common/Components/Transform.h"
#include "Common/Components/Kinematics.h"
#include "Common/Components/Collider.h"
#include "Common/Components/CarInput.h"
#include "Common/Components/CarEngine.h"
#include "Common/Components/Health.h"
#include "Common/Components/HealthBar.h"
#include "Common/Components/Gun.h"
#include "Common/Components/Trajectory.h"
#include "Common/Components/Particles.h"
#include "Common/Components/Sprite.h"
#include "Common/Components/RectShape.h"
#include "Common/Components/CameraTarget.h"
#include "Common/Components/Bullet.h"

std::shared_ptr<Entity> Prefab::createCar(const bool renderable) {
    Collider collider = Collider(sf::Vector2f(80, 40));

    sf::FloatRect bounds = collider.shape.getLocalBounds();
    collider.shape.setOrigin(bounds.width / 2, bounds.height / 2);

    std::shared_ptr<Entity> car = std::make_shared<Entity>(
        Layer::CarLayer,
        Transform(sf::Vector2f(0, 0), 0.f),
        Kinematics(),
        collider,
        Health(100, 100),
        HealthBar(sf::Vector2f(50, 10), sf::Color::Red, sf::Color::Green, 0.5f),
        CarEngine(1000, 1000 / 3, 200, 24, 800, 3.1415f / 3, 0.001f, sf::Vector2f(1, 0)),
        Gun(sf::Vector2f(1, 0), sf::seconds(0.1))
        );

    if (renderable) {
        sf::Sprite s = sf::Sprite(ResourceManager::getInstance()->textures()->get(Textures::Car));
        s.setScale(sf::Vector2f(0.132f, 0.132f));

        sf::FloatRect spriteBounds = s.getLocalBounds();
        s.setOrigin(spriteBounds.width / 2, spriteBounds.height / 2);

        car->addComponent<Sprite>(Sprite(s));
        car->addComponent<Particles>(Particles(sf::Vector2f(0, 0), sf::Color::White, sf::seconds(0.7), sf::seconds(1.0 / 40.0)));
        car->addComponent<Trajectory>(Trajectory());
    }

    return car;
}

std::shared_ptr<Entity> Prefab::createPlayableCar(const bool renderable) {
    std::shared_ptr<Entity> playableCar = Prefab::createCar(renderable);
    playableCar->addComponent<CarInput>(CarInput());
    playableCar->addComponent<CameraTarget>(CameraTarget());
    return playableCar;
}

std::shared_ptr<Entity> Prefab::createBullet(const bool renderable) {
    std::shared_ptr<Entity> bullet = std::make_shared<Entity>(
        Layer::ProjectileLayer,
        Transform(sf::Vector2f(0, 0), 0.f),
        Kinematics(),
        Collider(sf::Vector2f(5, 5)),
        Bullet(1, 1500, sf::seconds(1), nullptr)
        );

    if (renderable) {
        sf::Sprite s = sf::Sprite();
        s.setTexture(ResourceManager::getInstance()->textures()->get(Textures::Bullet));
        s.setScale(sf::Vector2f(0.5f, 0.5f));

        sf::FloatRect bounds = s.getLocalBounds();
        s.setOrigin(bounds.width / 2, bounds.height / 2);

        bullet->addComponent<Sprite>(Sprite(s));
    }

    return bullet;
}