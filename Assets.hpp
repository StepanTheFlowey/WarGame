#pragma once

#include <SFML/Graphics.hpp>
#include "main.hpp"

class Assets {
public:
  sf::Font font;
  sf::Texture texture;
  struct {
    bool fullscreen = false;
  } settings;

  inline Assets() {
    debug(L"Assets()");
  }

  inline ~Assets() {
    debug(L"~Assets()");
  }

  void load();

  void save() const;
};
extern Assets* assets;
