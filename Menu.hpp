#pragma once

#include "main.hpp"
#include <SFML/Graphics/Vertex.hpp>

class Menu {
  sf::Vertex gradient[4];
  uint16_t gradientDegree = 0;
public:

  inline Menu() {
    debug(L"Menu()");
  }

  inline ~Menu() {
    debug(L"~Menu()");
  }

  int run();

private:

  int broker(int button);

  int select();

  int settings();
};
