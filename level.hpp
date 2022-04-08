#pragma once

#include <SFML/Graphics.hpp>
#include "LevelStorage.hpp"

class Level : public sf::Drawable {
  sf::VertexBuffer vbo_;
public:

  Level() {
    debug(L"Level()");
    vbo_.setPrimitiveType(sf::Quads);
    vbo_.setUsage(sf::VertexBuffer::Static);
  }

  ~Level() {
    debug(L"Level()");
  }

  void load(const LevelStorage& storage);

  void save(LevelStorage& storage) const;

  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
extern Level* level;
