#include "Enemy.hpp"

#include "Randomizer.hpp"
#include "Context.hpp"
#include "Magazine.hpp"
#include "Level.hpp"

Enemy::Enemy(const sf::Vector2f position) {
  debug(L"Enemy()");

  health_ = 5;

  sprite_.setTexture(*context->getTexture(ID_IMG1));
  sprite_.setTextureRect(sf::IntRect(0, 0, 16, 16));
  sprite_.setScale(sf::Vector2f(4.F, 4.F));
  sprite_.setOrigin(sf::Vector2f(8.F, 8.F));
  sprite_.setPosition(position);
  sprite_.setColor(sf::Color::Red);

  healthBar.rect.setFillColor(sf::Color::Black);
  healthBar.rect.setSize(sf::Vector2f(54.F, 14.F));
  healthBar.rect.setOrigin(sf::Vector2f(27.F, 7.F));
  healthBar.rect.setPosition(position - sf::Vector2f(0.F, 40.F));

  healthBar.health.setFillColor(sf::Color::Red);
  healthBar.health.setSize(sf::Vector2f(50.F, 10.F));
  healthBar.health.setOrigin(sf::Vector2f(25.F, 5.F));
  healthBar.health.setPosition(position - sf::Vector2f(0.F, 40.F));
}

Enemy::~Enemy() {
  debug(L"~Enemy()");
}

void Enemy::fire() {
  float d = 0.F;
  if(to_underlying(direction_ & Direction::Back)) {
    d += 180.F;
  }
  if(to_underlying(direction_ & Direction::Left)) {
    if(to_underlying(direction_ & Direction::Back)) {
      d += 45.F;
    }
    else if(to_underlying(direction_ & Direction::Forward)) {
      d -= 45.F;
    }
    else {
      d -= 90.F;
    }
  }
  if(to_underlying(direction_ & Direction::Right)) {
    if(to_underlying(direction_ & Direction::Back)) {
      d -= 45.F;
    }
    else if(to_underlying(direction_ & Direction::Forward)) {
      d += 45.F;
    }
    else {
      d += 90.F;
    }
  }
  magazine->fire(sprite_.getPosition(), d, 1000);
}

const sf::FloatRect Enemy::getRect() {
  sf::Vector2f pos = sprite_.getPosition();
  return sf::FloatRect(pos.x - 16.F, pos.y - 24.F, 32.F, 50.F);
}

void Enemy::move(const sf::Vector2f& position) {
  sprite_.move(position);
  if(level->collide(sprite_.getGlobalBounds())) {
    sprite_.move(-position);
    walking_ = false;
  }
  else {
    const sf::Vector2f pos = sprite_.getPosition() - sf::Vector2f(0.F, 40.F);
    healthBar.health.setPosition(pos);
    healthBar.rect.setPosition(pos);
  }
}

void Enemy::damage(const int16_t amount) {
  Entity::damage(amount);

  healthBar.health.setSize(sf::Vector2f(10.F * health_, 10.F));
}

void Enemy::update(const sf::Time& time) {
  if(!walking_) {
    time_ -= time;
    if(time_.asMicroseconds() < 0) {
      time_ = sf::milliseconds(random->get() % 4000 + 1500);
      walking_ = false;
    }
    else {
      return;
    }

    switch(random->get() % 2) {
      case 0:
        direction_ = static_cast<Direction>(1 << (random->get() % 4));
        break;
      case 1:
        walking_ = true;
        break;
    }

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
    return;
  }

  time_ -= time;
  if(time_.asMicroseconds() < 0) {
    time_ = sf::milliseconds(random->get() % 1000 + 500);
    walking_ = false;
  }

  if(direction_ != Direction::Null) {
    animTime_ += time;
    if(animTime_.asMilliseconds() > 400) {
      animTime_ = sf::microseconds(0);
      ++anim_;
    }

    const float offset = static_cast<float>(time.asMilliseconds() / 5) / 4.F;
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
}

void Enemy::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(sprite_);
  target.draw(healthBar.rect);
  target.draw(healthBar.health);
}
