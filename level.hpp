#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "LevelStorage.hpp"
#include "Magazine.hpp"
#include "Enemy.hpp"

class Level : public sf::Drawable {
  sf::VertexBuffer vbo_;
  std::vector<Entity*> entities_;
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

  inline bool collide(const sf::FloatRect rect) {
    return !sf::FloatRect(0, 0, 2048, 2048).intersects(rect);
  }

  void update(sf::Time time);

  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
extern Level* level;
