#include <CopiableFromPrefab.h>
#include <PlayableCarInput.h>
#include "Common/Prefabs/Prefabs.h"

#include "Common/Managers/ResourceManager.h"

#include "Common/Components/Transform.h"
#include "Common/Components/Kinematics.h"
#include "Common/Components/Collider.h"
#include "Common/Components/VehicleInput.h"
#include "Common/Components/VehicleEngine.h"
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
        VehicleEngine(1000, 1000.0f / 3.0f, 200, 24, 800, 3.1415f / 3, 0.001f, sf::Vector2f(1, 0)),
        Gun(sf::Vector2f(1, 0), sf::seconds(0.1)),
        VehicleInput(),
        CopiableFromPrefab(Prefab::Type::Car)
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
    playableCar->addComponent<PlayableCarInput>(PlayableCarInput(playableCar->getComponent<VehicleInput>()));
    playableCar->addComponent<CameraTarget>(CameraTarget());
    playableCar->addComponent<CopiableFromPrefab>(CopiableFromPrefab(Prefab::Type::PlayableCar));
    return playableCar;
}

std::shared_ptr<Entity> Prefab::createBullet(const bool renderable) {
    std::shared_ptr<Entity> bullet = std::make_shared<Entity>(
        Layer::ProjectileLayer,
        Transform(sf::Vector2f(0, 0), 0.f),
        Kinematics(),
        Collider(sf::Vector2f(5, 5)),
        Bullet(1, 1500, sf::seconds(1), nullptr),
        CopiableFromPrefab(Prefab::Type::Bullet)
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

std::shared_ptr<Entity> Prefab::createWall(const bool renderable) {
    const float wallLength = 5000.0f;
    sf::RectangleShape shape(sf::Vector2f(wallLength, 10.0f));

    std::shared_ptr<Entity> wall = std::make_shared<Entity>(
            Transform(sf::Vector2f(0, wallLength/2.0f), 0.f),
            Collider(shape),
            CopiableFromPrefab(Prefab::Type::Wall)
    );

    if(renderable) {
        RectShape rect;
        rect.shape = shape;
        rect.shape.setFillColor(sf::Color::Blue);
        rect.shape.setOrigin(rect.shape.getSize().x / 2, rect.shape.getSize().y / 2);

        wall->addComponent(rect);
    }

    wall->setLayer(Layer::WallLayer);

    return wall;
}

std::shared_ptr<Entity> Prefab::create(Prefab::Type type, const bool renderable) {
    switch (type) {
        case Prefab::Type::Wall:
            return createWall(renderable);

        case Prefab::Type::PlayableCar:
            return createPlayableCar(renderable);

        case Prefab::Type::Car:
            return createCar(renderable);

        case Prefab::Type::Bullet:
            return createBullet(renderable);

        // si d'autres types de Prefab ont été rajoutés, les ajouter ici aussi
    }
}