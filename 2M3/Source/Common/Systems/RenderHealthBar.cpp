#include "RenderHealthBar.h"

void RenderHealthBar::update(const sf::Time&)
{
	for (Entity* entity: entities)
	{
		HealthBar* bar = entity->getComponent<HealthBar>();
		bar->background.setPosition(entity->getComponent<Transform>()->position + sf::Vector2f(0, 50));
		//target.draw(bar->background);
		float hpWidth = bar->background.getSize().x;
		float hpHeight = bar->background.getSize().y;

		Health* h = entity->getComponent<Health>();

		float hpBarWidth = hpWidth * h->health / (float)h->maxHealth;
		bar->bar.setPosition(bar->background.getPosition() - sf::Vector2f(hpWidth / 2.f, hpHeight / 2.f));
		bar->bar.setSize(sf::Vector2f(hpBarWidth, hpHeight));
		//target.draw(bar->bar);
	}
}