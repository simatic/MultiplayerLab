#include "GameState.h"
#include "Common/Managers/ResourceManager.h"
#include <Client/Managers/ClientGameManager.h>
#include "Common/Prefabs/Prefabs.h"

#include "Common/Systems/BulletSystem.h"
#include "Common/Systems/VehicleCollisionHandling.h"
#include "Common/Systems/VehicleDeath.h"
#include "Common/Systems/VehicleMovementSystem.h"
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
#include "Client/Systems/NetworkPingPong.h"

#include <iostream>

#include <SFML/Graphics/RenderWindow.hpp>
#include <Common/Components/CameraTarget.h>
#include <Common/Systems/CameraSystem.h>
#include <GridRenderer.h>
#include <Client/Systems/NetworkAddEntities.h>
#include <Client/Systems/NetworkSetTransform.h>
#include <Client/Systems/NetworkWorldState.h>
#include <Client/Systems/NetworkSetColor.h>
#include <Client/Systems/NetworkSetSpeed.h>
#include <Client/Systems/NetworkSendInputs.h>

GameState::GameState(StateStack& stack, Context context) :
	State(stack, context),
    networkModule(std::make_unique<ClientNetworkModule>(context, "localhost", DEFAULT_PORT)),
    gameManager(std::make_unique<ClientGameManager>(networkModule.get()))
{
	target = context.target;
	ResourceManager::getInstance()->setTextures(context.textures);
    gameManager->setRenderTarget(target);
    gameManager->setKeyBinding(context.keys);

    gameManager->addLogicSystems<
        KeyboardInputSystem,
        VehicleMovementSystem,
        CollisionSystem,
        VehicleCollisionSystem,
        BulletSystem,
        TrajectorySystem,
        ParticleSystem,
        VehicleDeath,
        MovementSystem
    >();
    
    gameManager->addRenderSystems<
        CameraSystem,
        GridRenderer,
        TrajectoryRenderer,
        ParticleRenderer,
        SpriteRenderer,
        RectShapeRenderer,
        HealthBarRenderer
    >();

    gameManager->addNetworkSystems<
        NetworkPingPong,
        NetworkWorldState,
        NetworkAddEntities,
        NetworkSetColor,
        NetworkSetTransform,
        NetworkSetSpeed,
        NetworkSendInputs
    >();

    gameManager->updateSystemListsForAllEntities();
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

