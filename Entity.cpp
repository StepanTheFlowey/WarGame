#include "Entity.hpp"

#include "Level.hpp"

void Entity::move(const sf::Vector2f& position) {
  sprite_.move(position);
  if(level->collide(sprite_.getGlobalBounds())) {
    sprite_.move(-position);
  }
}

void Entity::damage(const int16_t amount) {
  health_ -= amount;
  if(health_ <= 0) {
    health_ = 0;
    expired_ = true;
  }
}

void Entity::update(const sf::Time& time) {
  throw std::runtime_error("attempting to update pure entity");
}

void Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(sprite_);
}
