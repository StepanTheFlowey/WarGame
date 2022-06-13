#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include "main.hpp"

class Gui : public sf::Drawable {
public:

  Gui();

  ~Gui() = default;

  inline void setHealth(const int16_t health) {
    healthBar.rect.setSize(sf::Vector2f(20.F / health * 100.F, 20.F));
  }

  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
protected:

  struct {
    sf::RectangleShape rect;
    sf::RectangleShape health;
  } healthBar;
};
extern Gui* gui;
