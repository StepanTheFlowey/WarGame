#pragma once
#include "types.hpp"

struct Tile {
  enum class Type : uint8_t {
    None,
    Floor,
    Wall
  };
  Type type;
};