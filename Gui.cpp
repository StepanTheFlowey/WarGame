#include "Gui.hpp"

Dialogue* gui = nullptr;

Dialogue::Dialogue() {
  debug(L"Dialogue()");
  healthBar.rect.setPosition(sf::Vector2f(10, 10));
  healthBar.rect.setSize(sf::Vector2f(104, 24));
  healthBar.rect.setFillColor(sf::Color::Black);
  healthBar.health.setPosition(sf::Vector2f(12, 12));
  healthBar.health.setSize(sf::Vector2f(100, 20));
  healthBar.health.setFillColor(sf::Color::Red);
}

Dialogue::~Dialogue() {
  debug(L"~Dialogue()");
}

void Dialogue::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(healthBar.rect, states);
  target.draw(healthBar.health, states);
}
