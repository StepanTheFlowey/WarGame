#include "Entity.hpp"

Entity::Entity() {
  debug(L"Entity()");
}

Entity::~Entity() {
  debug(L"~Entity()");
}

void Entity::update() {
  debug(L"Entity::update()");
  throw;
}

void Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  debug(L"Entity::draw()");
  throw;
}
