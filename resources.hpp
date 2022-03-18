#pragma once
#include <SFML/Graphics.hpp>

#include "types.hpp"

class Resources {
  sf::Font font_;
  sf::Texture texture_;
public:

  Resources() {
    font_.loadFromFile("assets\\font\\fsys.ttf");
  }

  ~Resources() {

  }
};