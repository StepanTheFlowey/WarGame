#include "Player.hpp"

#include "Context.hpp"
#include "Magazine.hpp"
#include "Level.hpp"
#include "Gui.hpp"

Player* player = nullptr;

Player::Player() {
  sprite_.setTexture(*context->getTexture(ID_IMG1));
  sprite_.setTextureRect(sf::IntRect(0, 0, 16, 16));
  sprite_.setScale(sf::Vector2f(4.F, 4.F));
  sprite_.setOrigin(sf::Vector2f(8.F, 8.F));
}

void Player::setWalk(const Direction direction, const bool walk) {
  if(walk) {
    if(!walking_) {
      walking_ = true;
      direction_ = Direction::Null;
    }
    direction_ |= direction;
  }
  else {
    if(direction == direction_) {
      anim_ = 0;
      walking_ = false;
      switch(direction_) {
        case Direction::Forward:
          sprite_.setTextureRect(sf::IntRect(0, 32, 16, 16));
          break;
        case Direction::Back:
          sprite_.setTextureRect(sf::IntRect(0, 0, 16, 16));
          break;
        case Direction::Left:
          sprite_.setTextureRect(sf::IntRect(32, 16, 16, 16));
          break;
        case Direction::Right:
          sprite_.setTextureRect(sf::IntRect(0, 16, 16, 16));
          break;
      }
    }
    else {
      direction_ ^= direction;
    }
  }
}

void Player::fire() {
  float d = 0.F;
  if(to_underlying(direction_ & Direction::Back)) {
    d += 180.0F;
  }
  if(to_underlying(direction_ & Direction::Left)) {
    if(to_underlying(direction_ & Direction::Back)) {
      d += 45.0F;
    }
    else if(to_underlying(direction_ & Direction::Forward)) {
      d -= 45.0F;
    }
    else {
      d -= 90.0F;
    }
  }
  if(to_underlying(direction_ & Direction::Right)) {
    if(to_underlying(direction_ & Direction::Back)) {
      d -= 45.0F;
    }
    else if(to_underlying(direction_ & Direction::Forward)) {
      d += 45.0F;
    }
    else {
      d += 90.0F;
    }
  }
  magazine->fire(getPosition() + sf::Vector2f(sinf(d * F_DEG_TO_RAD) * 18.F, -cosf(d * F_DEG_TO_RAD) * 26.F), d, 1000);
}

const sf::FloatRect Player::getRect() {
  sf::Vector2f pos = sprite_.getPosition();
  return sf::FloatRect(pos.x - 16.F, pos.y - 24.F, 32.F, 50.F);
}

void Player::move(const sf::Vector2f& position) {
  sprite_.move(position);
  if(level->collide(getRect())) {
    sprite_.move(-position);
  }
}

void Player::damage(const int16_t amount) {
  Entity::damage(amount);

  gui->setHealth(health_);
}

void Player::update(const sf::Time& time) {
  if(!walking_) {
    return;
  }

  animTime_ += time;
  if(animTime_.asMilliseconds() > 100) {
    animTime_ = sf::microseconds(0);
    ++anim_;
  }
  const float offset = static_cast<float>(time.asMilliseconds() / 5);
  if(to_underlying(direction_ & Direction::Forward)) {
    move(sf::Vector2f(0.F, -offset));
    if(anim_ == 4) {
      anim_ = 0;
    }
    sprite_.setTextureRect(sf::IntRect(anim_ * 16, 32, 16, 16));
  }
  if(to_underlying(direction_ & Direction::Back)) {
    move(sf::Vector2f(0.F, offset));
    if(anim_ == 4) {
      anim_ = 0;
    }
    sprite_.setTextureRect(sf::IntRect(anim_ * 16, 0, 16, 16));
  }
  if(to_underlying(direction_ & Direction::Left)) {
    move(sf::Vector2f(-offset, 0.F));
    if(anim_ >= 2) {
      anim_ = 0;
    }
    sprite_.setTextureRect(sf::IntRect(anim_ * 16 + 32, 16, 16, 16));
  }
  if(to_underlying(direction_ & Direction::Right)) {
    move(sf::Vector2f(offset, 0.F));
    if(anim_ >= 2) {
      anim_ = 0;
    }
    sprite_.setTextureRect(sf::IntRect(anim_ * 16, 16, 16, 16));
  }
}
