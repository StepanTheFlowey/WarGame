#include "Entity.hpp"

Entity::Entity() {
  debug(L"Entity()");
}

Entity::~Entity() {
  debug(L"~Entity()");
}

const sf::FloatRect Entity::getRect() {
  return sf::FloatRect();
}

void Entity::damage(const int16_t amount) {
  health_ -= amount;
  if(health_ < 0) {
    expired_ = true;
  }
}

void Entity::update(sf::Time time) {

}

void Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(sprite_);
}
