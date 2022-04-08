#pragma once
#include <vector>

#include "main.hpp"
#include "tile.hpp"

class LevelStorage {
  std::vector<Tile*> tiles_;
  TilePos size_;
public:

  inline LevelStorage() {
    debug(L"LevelStorage()");
  }

  inline ~LevelStorage() {
    debug(L"~LevelStorage()");
  }

#ifdef DEBUG
  void loadTest();
#endif // DEBUG

  void load(const std::wstring filename);

  void save(const std::wstring filename);

  inline const TilePos getSize() const {
    return size_;
  }

  const Tile* getTile(const TilePos position) const;

  void setTile(const TilePos position, Tile* const tile);
};