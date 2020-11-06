#include <PlayerGUI.h>
#include <iostream>

namespace GUI
{
	PlayerGUI::PlayerGUI(const FontHolder& fonts) :
		mAction("Shoot", fonts),
		mPlayer(nullptr)
	{

	}

	PlayerGUI::PlayerGUI(Player* player, const FontHolder& fonts) :
		mPlayer(player),
		mAction("Shoot", fonts),
		mSpeedometer(),
		mSpeedometerBackground(),
		mMiniMap()
	{
		setPosition(0, 800);

		mAction.setPosition(getPosition());

		mSpeedometerBackground.setRadius(100);
		mSpeedometerBackground.setOrigin(100, 100);
		mSpeedometerBackground.setFillColor(sf::Color::White);
		mSpeedometerBackground.setPosition(getPosition() + sf::Vector2f(800, 100));

		mSpeedometer.setSize(sf::Vector2f(90, 5));
		mSpeedometer.setOrigin(0, 2.5f);
		mSpeedometer.setPosition(mSpeedometerBackground.getPosition());
		mSpeedometer.setRotation(180.f);
		mSpeedometer.setFillColor(sf::Color::Black);
	}

	void GUI::PlayerGUI::initialize(Player* player)
	{
		mPlayer = player;
		setPosition(0, 800);

		mAction.setPosition(getPosition());

		mSpeedometerBackground.setRadius(100);
		mSpeedometerBackground.setOrigin(100, 100);
		mSpeedometerBackground.setFillColor(sf::Color::White);
		mSpeedometerBackground.setPosition(getPosition() + sf::Vector2f(800, 100));

		mSpeedometer.setSize(sf::Vector2f(90, 5));
		mSpeedometer.setOrigin(0, 2.5f);
		mSpeedometer.setPosition(mSpeedometerBackground.getPosition());
		mSpeedometer.setRotation(180.f);
		mSpeedometer.setFillColor(sf::Color::Black);

		mMiniMap.setSize(sf::Vector2f(800, 450));
		if (mPlayer->getID() == 0) mMiniMap.setPosition(0, 225);
		else mMiniMap.setPosition(800, 225);
		mMiniMap.setFillColor(sf::Color(255, 255, 255, 200));
	}

	bool PlayerGUI::isSelectable() const
	{
		return false;
	}

	void PlayerGUI::handleEvent(const sf::Event&)
	{
	}

	void PlayerGUI::updateElements(sf::RenderTarget& target, std::vector<Entity*> entities, sf::Vector2f worldSize)
	{
		if (mPlayer != nullptr)
		{
			Car* car = mPlayer->getCar();
			if (mPlayer->getID() == 0) setPosition(target.mapPixelToCoords(sf::Vector2i(0, target.getSize().y*0.75)));
			else setPosition(target.mapPixelToCoords(sf::Vector2i(target.getSize().x * 0.5, target.getSize().y * 0.75))); // useless 'cuz there is only one player now

			mAction.setPosition(getPosition());
			mSpeedometerBackground.setPosition(getPosition() + sf::Vector2f(400, 100));
			mSpeedometer.setPosition(mSpeedometerBackground.getPosition());

			mAction.setText(car->getActionText());
			mSpeedometer.setRotation(180.f + 180.f * car->getSpeedRatio());

			mMiniMapShapes.clear();
			if (mPlayer->getCar()->getShowMap())
			{
				updateMap(entities, worldSize);
			}
		}
	}

	void GUI::PlayerGUI::updateMap(std::vector<Entity*> entities, sf::Vector2f worldSize)
	{
		for (auto& ent : entities)
		{
			switch (ent->getType())
			{
			case Entity::Type::CarType:
			{
				sf::ConvexShape triangle = sf::ConvexShape(3);
				triangle.setPoint(0, sf::Vector2f(0, 0));
				triangle.setPoint(1, sf::Vector2f(5, 2.5f));
				triangle.setPoint(2, sf::Vector2f(0, 5));
				triangle.setOrigin(2.5f, 2.5f);
				triangle.setFillColor(sf::Color::Red);
				triangle.setPosition(ent->getMiniMapPosition(worldSize, mMiniMap.getSize()) + mMiniMap.getPosition());
				triangle.setRotation(ent->getRotation());
				mMiniMapShapes.push_back(triangle);
			}

			default:
				break;
			}
		}
	}

	void PlayerGUI::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		//states.transform *= getTransform();
		if (mPlayer != nullptr)
		{
			target.draw(mAction, states);
			target.draw(mSpeedometerBackground, states);
			target.draw(mSpeedometer, states);

			if (mPlayer->getCar()->getShowMap())
			{
				target.draw(mMiniMap);
				for (auto& shape : mMiniMapShapes)
				{
					target.draw(shape);
				}
			}
		}
	}

}