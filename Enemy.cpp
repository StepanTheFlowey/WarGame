#include "Enemy.hpp"

#include "Context.hpp"
#include "Magazine.hpp"
#include "Level.hpp"

Enemy::Enemy(const sf::Vector2f position) {
  debug(L"Enemy()");
  sprite_.setTexture(*context->getTexture(ID_IMG1));
  sprite_.setTextureRect(sf::IntRect(0, 0, 16, 16));
  sprite_.setScale(sf::Vector2f(4.0F, 4.0F));
  sprite_.setOrigin(sf::Vector2f(8.0F, 8.0F));
  sprite_.setPosition(position);
  sprite_.setColor(sf::Color::Red);
}

Enemy::~Enemy() {
  debug(L"~Enemy()");
}

void Enemy::fire() {
  float d = 0.0F;
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
  magazine->fire(sprite_.getPosition(), d, 1000);
}


void Enemy::update(sf::Time time) {
  if(!walking_) {
    time_ -= time;
    if(time_.asMicroseconds() < 0) {
      time_ = sf::milliseconds(rand() % 2000 + 1000);
      walking_ = false;
    }
    else {
      return;
    }

    switch(rand() % 2) {
      case 0:
        direction_ = static_cast<Direction>(1 << (rand() % 4));
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
    time_ = sf::milliseconds(rand() % 2000 + 1000);
    walking_ = false;
  }

  animTime_ += time;
  if(animTime_.asMilliseconds() > 400) {
    animTime_ = sf::microseconds(0);
    ++anim_;
  }
  const float offset = static_cast<float>(time.asMilliseconds()) / 20;
  sf::Vector2f m;
  if(to_underlying(direction_ & Direction::Forward)) {
    m += sf::Vector2f(0.0F, -offset);
    if(anim_ == 4) {
      anim_ = 0;
    }
    sprite_.setTextureRect(sf::IntRect(anim_ * 16, 32, 16, 16));
  }
  if(to_underlying(direction_ & Direction::Back)) {
    m += sf::Vector2f(0.0F, offset);
    if(anim_ == 4) {
      anim_ = 0;
    }
    sprite_.setTextureRect(sf::IntRect(anim_ * 16, 0, 16, 16));
  }
  if(to_underlying(direction_ & Direction::Left)) {
    m += sf::Vector2f(-offset, 0.0F);
    if(anim_ >= 2) {
      anim_ = 0;
    }
    sprite_.setTextureRect(sf::IntRect(anim_ * 16 + 32, 16, 16, 16));
  }
  if(to_underlying(direction_ & Direction::Right)) {
    m += sf::Vector2f(offset, 0.0F);
    if(anim_ >= 2) {
      anim_ = 0;
    }
    sprite_.setTextureRect(sf::IntRect(anim_ * 16, 16, 16, 16));
  }
  sprite_.move(m);
  if(level->collide(sprite_.getGlobalBounds())) {
    sprite_.move(-m);
    walking_ = false;
  }
}

void Enemy::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(sprite_);
  target.draw(healthBar.rect);
  target.draw(healthBar.health);
}
