#include "Level.hpp"

#include "Context.hpp"
#include "Player.hpp"
#include <fstream>
#include <WinSock2.h>

Level* level = nullptr;

Level::Level() {
  vbo_.setPrimitiveType(sf::Quads);
  vbo_.setUsage(sf::VertexBuffer::Static);
}

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

  if(file.read(reinterpret_cast<char*>(&sizes_), sizeof(sizes_)).gcount() != sizeof(sizes_)) {
    return false;
  }
  sizes_.x = ntohs(sizes_.x);
  sizes_.y = ntohs(sizes_.y);

  const TileIndex size = sizes_.x * sizes_.y;
  tiles_.resize(size);

  if(file.read(reinterpret_cast<char*>(tiles_.data()), size * sizeof(Tile)).gcount() != size * sizeof(Tile)) {
    return false;
  }

  sf::Vertex* const vt = new sf::Vertex[size * 4];

  TileIndex index;
  sf::Vector2f pos;
  sf::Vector2f tex;
  for(TileInt i = 0; i < sizes_.y; ++i) {
    for(TileInt j = 0; j < sizes_.x; ++j) {
      index = (j + i * sizes_.x) * 4;

      pos.x = static_cast<float>(j);
      pos.y = static_cast<float>(i);
      pos *= S_TILE;

      vt[index + 0].position = pos + sf::Vector2f(0.F, 0.F);
      vt[index + 1].position = pos + sf::Vector2f(0.F, S_TILE);
      vt[index + 2].position = pos + sf::Vector2f(S_TILE, S_TILE);
      vt[index + 3].position = pos + sf::Vector2f(S_TILE, 0.F);

      tex = getTileTexturePos(tiles_[index / 4].type) + sf::Vector2f(0.F, 48.F);
      vt[index + 0].texCoords = tex + sf::Vector2f(0.F, 0.F);
      vt[index + 1].texCoords = tex + sf::Vector2f(0.F, S_TEXTURE);
      vt[index + 2].texCoords = tex + sf::Vector2f(S_TEXTURE, S_TEXTURE);
      vt[index + 3].texCoords = tex + sf::Vector2f(S_TEXTURE, 0.F);

      switch(tiles_[index / 4].type) {
        case Tile::Type::Player:
          player->setPosition(pos + sf::Vector2f(S_TILE / 2.F, S_TILE / 2.F));
          break;
        case Tile::Type::Enemy:
          entities_.push_back(new Enemy(pos + sf::Vector2f(S_TILE / 2.F, S_TILE / 2.F)));
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

bool Level::collide(const sf::FloatRect& rect) {
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

Entity* Level::collideEntity(const sf::FloatRect& rect) {
  if(player->getRect().intersects(rect)) {
    return player;
  }

  for(auto i : entities_) {
    if(i->getRect().intersects(rect)) {
      return i;
    }
  }

  return nullptr;
}

bool Level::collideOutside(const sf::FloatRect& rect) {
  return !sf::FloatRect(0.F, 0.F, sizes_.x * 64.F, sizes_.y * 64.F).intersects(rect);
}

void Level::update(const sf::Time& time) {
  for(size_t i = 0; i < entities_.size(); ++i) {
    if(entities_[i]->expired()) {
      entities_.erase(entities_.begin() + i);
      --i;
      continue;
    }

    entities_[i]->update(time);
  }
}

void Level::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(vbo_, context->getTexture(ID_IMG1));

  for(const auto& i : entities_) {
    target.draw(*i);
  }
}
