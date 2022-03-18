#pragma once
#include <vector>

#include "types.hpp"
#include "tile.hpp"

class LevelStorage {
  std::vector<Tile> tiles_;
public:
  LevelStorage() {

  }

  ~LevelStorage() {

  }

  void loadFromFile(const std::wstring filename) {
    
  }
};