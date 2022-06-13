#include "Level.hpp"

#include "Context.hpp"
#include "Player.hpp"
#include <fstream>
#include <WinSock2.h>

Level* level = nullptr;

void rewindString(std::ifstream& file) {
  while(file.get() != 0) {

  }
}

bool Level::load(const LevelInfo& info) {
  info_ = info;

  std::ifstream file(info_.filename_);

  if(!file) {
    return false;
  }

  file.seekg(6);
  rewindString(file);
  rewindString(file);

  TileInt w = 0;
  if(file.read(reinterpret_cast<char*>(&w), sizeof(w)).gcount() != sizeof(w)) {
    return false;
  }
  TileInt h = 0;
  if(file.read(reinterpret_cast<char*>(&h), sizeof(h)).gcount() != sizeof(h)) {
    return false;
  }
  sizes_.x = ntohs(w);
  sizes_.y = ntohs(h);

  const TileIndex size = sizes_.x * sizes_.y;
  tiles_.resize(size);

  if(file.read(reinterpret_cast<char*>(tiles_.data()), size * sizeof(Tile)).gcount() != size * sizeof(Tile)) {
    return false;
  }

  constexpr float tileSize = 64.F;
  constexpr float texSize = 16.F;

  sf::Vertex* const vt = new sf::Vertex[size * 4];

  TileIndex index;
  sf::Vector2f pos;
  sf::Vector2f tex;
  for(TileInt i = 0; i < sizes_.y; ++i) {
    for(TileInt j = 0; j < sizes_.x; ++j) {
      index = (j + i * sizes_.x) * 4;

      pos.x = static_cast<float>(j);
      pos.y = static_cast<float>(i);
      pos *= tileSize;

      vt[index + 0].position = pos + sf::Vector2f(0.F, 0.F);
      vt[index + 1].position = pos + sf::Vector2f(0.F, tileSize);
      vt[index + 2].position = pos + sf::Vector2f(tileSize, tileSize);
      vt[index + 3].position = pos + sf::Vector2f(tileSize, 0.F);

      tex = getTileTexturePos(tiles_[index / 4].type) + sf::Vector2f(0.F, 48.F);
      vt[index + 0].texCoords = tex + sf::Vector2f(0.F, 0.F);
      vt[index + 1].texCoords = tex + sf::Vector2f(0.F, texSize);
      vt[index + 2].texCoords = tex + sf::Vector2f(texSize, texSize);
      vt[index + 3].texCoords = tex + sf::Vector2f(texSize, 0.F);

      switch(tiles_[index / 4].type) {
        case Tile::Type::Player:
          player->setPosition(pos + sf::Vector2f(tileSize / 2, tileSize / 2));
          break;
        case Tile::Type::Enemy:
          entities_.push_back(new Enemy(pos + sf::Vector2f(tileSize / 2, tileSize / 2)));
          break;
      }
    }
  }

  vbo_.create(static_cast<size_t>(size) * 4);
  vbo_.update(vt);
  delete[] vt;

  return true;
}

void Level::save() const {

}

bool Level::collide(const sf::FloatRect rect) {
  TileIndex index;
  for(TileInt i = 0; i < sizes_.y; ++i) {
    for(TileInt j = 0; j < sizes_.x; ++j) {
      index = j + i * sizes_.x;

      if(tiles_[index].type == Tile::Type::Wall) {
        if(sf::FloatRect(j * 64.F, i * 64.F, 64.F, 64.F).intersects(rect)) {
          return true;
        }
      }
    }
  }
  return false;
}

void Level::update(sf::Time time) {
  for(auto& i : entities_) {
    i->update(time);
  }
}

void Level::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(vbo_, context->getTexture(ID_IMG1));

  for(const auto& i : entities_) {
    target.draw(*i);
  }
}
