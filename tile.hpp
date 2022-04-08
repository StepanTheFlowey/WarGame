#pragma once

#include "types.hpp"
#include <SFML/System/Vector2.hpp>

struct Tile {
  enum class Type : uint8_t {
    None,
    Floor,
    Wall
  };
  Type type;
};
using TilePos = sf::Vector2<uint16_t>;