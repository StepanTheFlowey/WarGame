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

  void loadTest();

  void load(const std::wstring filename);

  void loadTiles();

  void save(const std::wstring filename);

  inline const TilePos getSize() const {
    return size_;
  }

  const Tile* getTile(const TilePos position) const;

  void setTile(const TilePos position, Tile* const tile);
};