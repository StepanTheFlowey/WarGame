#pragma once

#include <SFML/Graphics.hpp>
#include "main.hpp"

class Gui :public sf::Drawable {
  struct {
    sf::RectangleShape rect;
    sf::RectangleShape health;
  } healthBar;
public:

  Gui();

  ~Gui();

  inline void setHealth(const uint8_t health) {
    healthBar.rect.setSize(sf::Vector2f(20.0F / health * 100, 20.0F));
  }

  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
extern Gui* gui;