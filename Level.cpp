#include "Level.hpp"

Level* level = nullptr;

void Level::load(const LevelStorage& storage) {
  TilePos size = storage.getSize();

  std::vector<sf::Vertex> vt(static_cast<size_t>(size.x) * size.y * 4);

  size_t index;
  for(uint16_t i = 0; i < size.x; ++i) {
    for(uint16_t j = 0; j < size.y; ++j) {
      index = (static_cast<size_t>(i) + j * size.x) * 4;

      vt[index + 0] = sf::Vertex(
        sf::Vector2f(i * 64, j * 64), sf::Color(rand(), rand(), rand()));
      vt[index + 1] = sf::Vertex(
        sf::Vector2f(i * 64, j * 64 + 64), sf::Color(rand(), rand(), rand()));
      vt[index + 2] = sf::Vertex(
        sf::Vector2f(i * 64 + 64, j * 64 + 64), sf::Color(rand(), rand(), rand()));
      vt[index + 3] = sf::Vertex(
        sf::Vector2f(i * 64 + 64, j * 64), sf::Color(rand(), rand(), rand()));
    }
  }
}

void Level::save(LevelStorage& storage) const {

}

void Level::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(vbo_);
}
