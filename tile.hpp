#pragma once

#include "main.hpp"
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
using TilePos = sf::Vector2<uint16_t>;
