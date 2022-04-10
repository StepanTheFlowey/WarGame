#include "LevelStorage.hpp"

void LevelStorage::loadTest() {
  tiles_.resize(32 * 32);
  for(auto& i : tiles_) {
    i = new Tile;
  }
  size_.x = 32;
  size_.y = 32;
}

void LevelStorage::load(const std::wstring filename) {

}

void LevelStorage::loadTiles() {

}

void LevelStorage::save(const std::wstring filename) {

}

const Tile* LevelStorage::getTile(const TilePos position) const {
  if(position.x >= size_.x || position.y >= size_.y) return nullptr;
  return tiles_.at(static_cast<size_t>(position.x + position.y * size_.x));
}

void LevelStorage::setTile(const TilePos position, Tile* const tile) {
  if(position.x >= size_.x || position.y >= size_.y) return;
  tiles_[static_cast<size_t>(position.x + position.y * size_.x)] = tile;
}
