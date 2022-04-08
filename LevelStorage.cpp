#include "LevelStorage.hpp"

#ifdef DEBUG
void LevelStorage::loadTest() {
  tiles_.resize(64 * 64);
  for(auto& i : tiles_) {
    i = new Tile;
  }
}
#endif // DEBUG

void LevelStorage::load(const std::wstring filename) {

}

void LevelStorage::save(const std::wstring filename) {

}

const Tile* LevelStorage::getTile(const TilePos position) const {
  if(position.x >= size_.x || position.y >= size_.y) return;
  return tiles_.at(position.x + position.y * size_.x);
}

void LevelStorage::setTile(const TilePos position, Tile* const tile) {
  if(position.x >= size_.x || position.y >= size_.y) return;
  tiles_[position.x + position.y * size_.x] = tile;
}
