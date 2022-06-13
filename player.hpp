#pragma once

#include "Entity.hpp"
#include "Direction.hpp"

class Player : public Entity {
  Direction direction_ = Direction::Null;
  bool walking_ = false;

  uint8_t anim_ = 0;
  sf::Time animTime_;
public:

  Player();

  virtual ~Player() = default;

  void fire();

  void setWalk(const Direction direction, const bool walk);

  virtual const sf::FloatRect getRect() override;

  virtual void move(const sf::Vector2f& position);

  virtual void damage(const int16_t amount) override;

  virtual void update(const sf::Time& time) override;
};
extern Player* player;
