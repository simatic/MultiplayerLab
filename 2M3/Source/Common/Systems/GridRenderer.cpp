#include <ResourceManager.h>
#include "Common/Systems/GridRenderer.h"
#include "Common/Managers/GameManager.h"

GridRenderer::GridRenderer(GameManager* const gameManager) :
	RenderSystem<Transform>(gameManager), gridElement(ResourceManager::getInstance()->textures()->get(Textures::GridElement))
{
    auto renderTarget = gameManager->getRenderTarget();
    auto view = renderTarget->getView();
    auto size = view.getSize();

    // +2 to ensure all the screen is covered
    int xCrossesCount = ceil(size.x*CROSSES_PER_UNIT)+2;
    int yCrossesCount = ceil(size.y*CROSSES_PER_UNIT)+2;

    grid.setPrimitiveType(sf::PrimitiveType::Quads);
    grid.resize(xCrossesCount*yCrossesCount*4);

    const float crossSize = 5.0f;

    for (int y = 0; y < yCrossesCount; y++) {
        for (int x = 0; x < xCrossesCount; x++) {
            auto screenPositionX = x/CROSSES_PER_UNIT;
            auto screenPositionY = y/CROSSES_PER_UNIT;

            auto& topLeftVertex = grid[(y*xCrossesCount+x)*4 + 0];
            auto& topRightVertex = grid[(y*xCrossesCount+x)*4 + 1];
            auto& bottomRightVertex = grid[(y*xCrossesCount+x)*4 + 2];
            auto& bottomLeftVertex = grid[(y*xCrossesCount+x)*4 + 3];
            topLeftVertex.position = {screenPositionX, screenPositionY};
            topRightVertex.position = {screenPositionX+crossSize, screenPositionY};
            bottomRightVertex.position = {screenPositionX+crossSize, screenPositionY+crossSize};
            bottomLeftVertex.position = {screenPositionX, screenPositionY+crossSize};

            topLeftVertex.texCoords = {0,0};
            topRightVertex.texCoords = {static_cast<float>(gridElement.getSize().x),0};
            bottomRightVertex.texCoords = {static_cast<float>(gridElement.getSize().x),static_cast<float>(gridElement.getSize().y)};
            bottomLeftVertex.texCoords = {0,static_cast<float>(gridElement.getSize().y)};
        }
    }
}

void GridRenderer::update(const sf::Time&)
{
    // setup texture
    sf::RenderStates states{};
    states.texture = &gridElement;
    auto view = gameManager->getRenderTarget()->getView();

    // offset grid to always be on screen
    transform = sf::Transform::Identity;
    sf::Vector2f topLeft = view.getCenter() - view.getSize()/2.0f;
    float offsetX;
    float offsetY;
    modff(topLeft.x*CROSSES_PER_UNIT, &offsetX);
    modff(topLeft.y*CROSSES_PER_UNIT, &offsetY);

    float gridPosX = floor(offsetX)/CROSSES_PER_UNIT;
    float gridPosY = floor(offsetY)/CROSSES_PER_UNIT;

    transform.translate(gridPosX, gridPosY);

    states.transform *= transform;

    // draw grid
    gameManager->getRenderTarget()->draw(grid, states);
}
