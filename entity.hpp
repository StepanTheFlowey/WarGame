#pragma once

#include <SFML/Graphics.hpp>
#include "main.hpp"

class Entity : public sf::Drawable {
  sf::Sprite sprite_;
public:

  Entity();

  virtual ~Entity();

  virtual void update();

  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};