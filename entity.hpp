#pragma once

#include <SFML/Graphics.hpp>
#include "main.hpp"

class Entity : public sf::Drawable {
public:

  inline Entity() {
    debug(L"Entity()");
  }

  inline virtual ~Entity() {
    debug(L"~Entity()");
  }

  inline virtual const sf::Vector2f& getPosition() {
    return sprite_.getPosition();
  }

  inline virtual void setPosition(const sf::Vector2f& position) {
    sprite_.setPosition(position);
  }

  inline virtual const sf::FloatRect getRect() {
    return sprite_.getGlobalBounds();
  }

  virtual void move(const sf::Vector2f& position);

  virtual void damage(const int16_t amount);

  inline virtual bool expired() const {
    return expired_;
  }

  virtual void update(const sf::Time& time);

  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
protected:

  bool expired_ = false;
  int16_t health_ = 0;
  sf::Sprite sprite_;
};
