#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "LevelInfo.hpp"
#include "Magazine.hpp"
#include "Enemy.hpp"
#include "Tile.hpp"

class Level : public sf::Drawable {
  sf::VertexBuffer vbo_;
  std::vector<Entity*> entities_;
  std::vector<Tile*> tiles_;
  LevelInfo info_;
public:

  Level() {
    debug(L"Level()");
    vbo_.setPrimitiveType(sf::Quads);
    vbo_.setUsage(sf::VertexBuffer::Static);
  }

  ~Level() {
    debug(L"Level()");
  }

  void load(LevelInfo& storage);

  void save() const;

  bool collide(const sf::FloatRect rect);

  void update(sf::Time time);

  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
extern Level* level;
