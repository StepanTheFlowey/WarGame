#pragma once

#include "main.hpp"
#include <SFML/Graphics.hpp>

class Bullet : public sf::Drawable {
  sf::Sprite sprite_;
  sf::Vector2f velocity_;
  int32_t lenght_;
  bool expired_ = false;
public:

  Bullet(const sf::Vector2f begin, const float degree, const uint32_t lenght);

  ~Bullet();

  inline bool expired() {
    return expired_;
  }

  void update(sf::Time time);

  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
