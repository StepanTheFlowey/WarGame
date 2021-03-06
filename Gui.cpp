#include "Gui.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

Gui* gui = nullptr;

Gui::Gui() {
  healthBar.rect.setPosition(sf::Vector2f(10, 10));
  healthBar.rect.setSize(sf::Vector2f(104, 24));
  healthBar.rect.setFillColor(sf::Color::Black);
  healthBar.health.setPosition(sf::Vector2f(12, 12));
  healthBar.health.setSize(sf::Vector2f(100, 20));
  healthBar.health.setFillColor(sf::Color::Red);
}

void Gui::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(healthBar.rect, states);
  target.draw(healthBar.health, states);
}
