#pragma once
#include <Label.h>
#include <Player.h>

//extern class Player;

namespace GUI
{

	class PlayerGUI : public Component
	{
	public:
		PlayerGUI(Player* player, const FontHolder& fonts);
		PlayerGUI(const FontHolder& fonts);
		void initialize(Player* player);

		bool isSelectable() const;
		void handleEvent(const sf::Event& event);

		void updateElements(sf::RenderTarget& target, std::vector<Entity*> entities, sf::Vector2f worldSize);
		void updateMap(std::vector<Entity*> entities, sf::Vector2f worldSize);

	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	private:
		Player* mPlayer;
		sf::RectangleShape mSpeedometer;
		sf::CircleShape mSpeedometerBackground;
		Label mAction;

		std::vector<sf::ConvexShape> mMiniMapShapes;
		sf::RectangleShape mMiniMap;

	};

}