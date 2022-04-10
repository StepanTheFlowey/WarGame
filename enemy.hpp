#pragma once
#include <SFML/Graphics.hpp>

#include "Entity.hpp"
#include "Direction.hpp"

class Enemy : public Entity {
  struct {
    sf::RectangleShape rect;
    sf::RectangleShape health;
  } healthBar;

  Direction direction_ = Direction::Null;
  bool walking_ = false;

  uint8_t anim_ = 0;
  sf::Time animTime_;

  sf::Time time_;
public:

  Enemy(const sf::Vector2f position);

  virtual ~Enemy() override;

  void fire();

  virtual void update(sf::Time time) override;

  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};