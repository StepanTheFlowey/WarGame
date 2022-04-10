#pragma once

#include <SFML/Graphics.hpp>
#include "main.hpp"

class Entity : public sf::Drawable {
  int16_t health_ = 0;
public:

  Entity();

  virtual ~Entity();

  virtual const sf::FloatRect getRect();

  virtual void damage(const int16_t amount);

  inline virtual bool expired() const {
    return expired_;
  }

  virtual void update(sf::Time time);

  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
protected:

  sf::Sprite sprite_;
  bool expired_;
};