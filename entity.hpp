#pragma once

#include <SFML/Graphics.hpp>
#include "main.hpp"

class Entity : public sf::Drawable {
  int16_t health_ = 0;
public:

  Entity();

  virtual ~Entity();

  virtual const sf::Vector2f& getPosition();

  virtual void setPosition(const sf::Vector2f& position);

  virtual const sf::FloatRect getRect();

  virtual void move(const sf::Vector2f& position);

  virtual void damage(int16_t amount);

  inline virtual bool expired() const {
    return expired_;
  }

  virtual void update(sf::Time time);

  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
protected:

  sf::Sprite sprite_;
  bool expired_ = false;
};