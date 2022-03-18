#pragma once
#include <SFML/Graphics.hpp>
#include "types.hpp"

class Entity : public sf::Drawable {
public:
  Entity() {

  }

  ~Entity() {

  }

  virtual void update();

  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};