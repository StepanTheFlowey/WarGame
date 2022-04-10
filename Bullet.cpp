#include "Bullet.hpp"

#include "Context.hpp"
#include <cmath>

Bullet::Bullet(const sf::Vector2f begin, const float degree, const uint32_t lenght) {
  debug(L"Bullet()");
  sprite_.setTexture(*context->getTexture(ID_IMG1));
  sprite_.setTextureRect(sf::IntRect(64, 0, 16, 16));
  sprite_.setPosition(begin);
  sprite_.setOrigin(sf::Vector2f(8.0F, 8.0F));
  velocity_.x = sinf(degree * F_DEG_TO_RAD);
  velocity_.y = -cosf(degree * F_DEG_TO_RAD);
  lenght_ = lenght;
}

Bullet::~Bullet() {
  debug(L"~Bullet()");
}

void Bullet::update(sf::Time time) {
  lenght_ -= time.asMilliseconds();

  const float factor = time.asMilliseconds();
  sf::Vector2f m = velocity_;
  m.x *= factor;
  m.y *= factor;
  sprite_.move(m);

  if(lenght_ < 0) {
    expired_ = true;
  }
}

void Bullet::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(sprite_);
}
