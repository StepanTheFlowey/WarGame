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
  std::vector<Tile> tiles_;
  LevelInfo info_;
  sf::Vector2<TileInt> sizes_;
public:

  Level();

  ~Level() = default;

  bool load(const LevelInfo& info);

  void save() const;

  bool collide(const sf::FloatRect& rect);

  Entity* collideEntity(const sf::FloatRect& rect);

  bool collideOutside(const sf::FloatRect& rect);

  void update(const sf::Time& time);

  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
extern Level* level;
