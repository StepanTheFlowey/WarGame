#include "Level.hpp"

Level* level = nullptr;

void Level::load(LevelInfo& storage) {

}

void Level::save() const {

}

bool Level::collide(const sf::FloatRect rect) {
  return false;
}

void Level::update(sf::Time time) {
  for(auto& i : entities_) {
    i->update(time);
  }
}

void Level::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(vbo_);

  for(const auto& i : entities_) {
    target.draw(*i);
  }
}
