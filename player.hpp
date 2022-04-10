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

  virtual ~Player() override;

  inline const sf::Vector2f getPosition() {
    return sprite_.getPosition();
  }

  void setWalk(const Direction direction, const bool walk);

  void fire();

  virtual void update(sf::Time time) override;
};
extern Player* player;
