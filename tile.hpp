#pragma once

#include <inttypes.h>
#include <SFML/System/Vector2.hpp>

struct Tile {
  enum class Type : uint8_t {
    None,
    Floor,
    Wall,
    Enemy
  };
  Type type;
};
using TileInt = uint16_t;
using TilePos = sf::Vector2<TileInt>;
using TileIndex = uint32_t;
