#include "Entity.hpp"

#include "Level.hpp"

Entity::Entity() {
  debug(L"Entity()");
}

Entity::~Entity() {
  debug(L"~Entity()");
}

const sf::Vector2f& Entity::getPosition() {
  return sprite_.getPosition();
}

void Entity::setPosition(const sf::Vector2f& position) {
  sprite_.setPosition(position);
}

const sf::FloatRect Entity::getRect() {
  return sprite_.getGlobalBounds();
}

void Entity::move(const sf::Vector2f& position) {
  sprite_.move(position);
  if(level->collide(sprite_.getGlobalBounds())) {
    sprite_.move(-position);
  }
}

void Entity::damage(int16_t amount) {
  health_ -= amount;
  if(health_ < 0) {
    expired_ = true;
  }
}

void Entity::update(sf::Time time) {
  throw std::runtime_error("attempting to update pure entity");
}

void Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(sprite_);
}
