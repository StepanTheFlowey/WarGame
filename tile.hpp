#pragma once

#include <inttypes.h>
#include <SFML/System/Vector2.hpp>

struct Tile {
  enum class Type : uint8_t {
    None,
    Floor,
    Wall,
    Player,
    Enemy
  };
  Type type;
};

using TileInt = uint16_t;
using TilePos = sf::Vector2<TileInt>;
using TileIndex = uint32_t;

inline sf::Vector2f getTileTexturePos(const Tile::Type type) {
  switch(type) {
    case Tile::Type::None:
      throw std::runtime_error("Tile::Type::None!");
      break;
    case Tile::Type::Floor:
      return sf::Vector2f(0.F, 0.F);
      break;
    case Tile::Type::Wall:
      return sf::Vector2f(16.F, 0.F);
      break;
    case Tile::Type::Player:
      return sf::Vector2f(48.F, 0.F);
      break;
    case Tile::Type::Enemy:
      return sf::Vector2f(32.F, 0.F);
      break;
  }
  throw std::runtime_error("Tile::Type::None!");
}
