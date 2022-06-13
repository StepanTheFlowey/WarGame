#include "Bullet.hpp"

#include "Context.hpp"
#include <cmath>

Bullet::Bullet(const sf::Vector2f& begin, const float degree, const int32_t lifetime) {
  debug(L"Bullet()");

  sprite_.setTexture(*context->getTexture(ID_IMG1));
  sprite_.setTextureRect(sf::IntRect(64, 0, 16, 16));
  sprite_.setPosition(begin);
  sprite_.setOrigin(sf::Vector2f(8.0F, 8.0F));
  velocity_.x = sinf(degree * F_DEG_TO_RAD);
  velocity_.y = -cosf(degree * F_DEG_TO_RAD);
  lifetime_ = lifetime;
}

Bullet::~Bullet() {
  debug(L"~Bullet()");
}

void Bullet::update(sf::Time time) {
  lifetime_ -= time.asMilliseconds();

  const float factor = static_cast<float>(time.asMilliseconds());
  sprite_.move(velocity_ * factor);

  if(lifetime_ < 0) {
    expired_ = true;
  }
}

void Bullet::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(sprite_);
}
