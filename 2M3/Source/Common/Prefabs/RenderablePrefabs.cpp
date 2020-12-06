#include "Common/Prefabs/RenderablePrefabs.h"

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

std::shared_ptr<Entity> Prefab::createRenderableCar() {
    Collider collider = Collider(sf::Vector2f(80, 40));

    sf::FloatRect bounds = collider.shape.getLocalBounds();
    collider.shape.setOrigin(bounds.width / 2, bounds.height / 2);

    sf::Sprite s = sf::Sprite(ResourceManager::getInstance()->textures()->get(Textures::Car));
    s.setScale(sf::Vector2f(0.132f, 0.132f));

    sf::FloatRect spriteBounds = s.getLocalBounds();
    collider.shape.setOrigin(spriteBounds.width / 2, spriteBounds.height / 2);
    s.setOrigin(spriteBounds.width / 2, spriteBounds.height / 2);

    return std::make_shared<Entity>(
        Transform(sf::Vector2f(0, 0), 0.f),
        Kinematics(),
        collider,
        Health(75, 100),
        HealthBar(sf::Vector2f(100, 10), sf::Color::Red, sf::Color::Green, 0.5f),
        CarEngine(1000, 1000 / 3, 200, 24, 800, 3.1415f / 3, 0.001f, sf::Vector2f(1, 0)),
        Gun(sf::Vector2f(1, 0), sf::seconds(0.1)),
        Trajectory(),
        Particles(sf::Vector2f(0, 0), sf::Color::White, sf::seconds(0.7), sf::seconds(1.0 / 40.0)),
        Sprite(s)
        );
}

std::shared_ptr<Entity> Prefab::createPlayableCar() {
    std::shared_ptr<Entity> playableCar = Prefab::createRenderableCar();
    playableCar->addComponent<CarInput>(CarInput());
    playableCar->addComponent<CameraTarget>(CameraTarget());
    return playableCar;
}