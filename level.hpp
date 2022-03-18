#pragma once
#include <SFML/Graphics.hpp>
#include "types.hpp"

class Level {
  sf::VertexBuffer vbo_;
public:

  Level() {
    vbo_.setPrimitiveType(sf::Quads);
    vbo_.setUsage(sf::VertexBuffer::Static);
  }

  ~Level() {

  }
};