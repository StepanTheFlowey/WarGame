#pragma once
#include <SFML/Graphics.hpp>
#include "types.hpp"

class Entity : public sf::Drawable {
public:
  virtual void update() = NULL;

  virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const = NULL;
};