#pragma once

#include <vector>
#include "Bullet.hpp"

class Magazine : public sf::Drawable {
  std::vector<Bullet*> bullets_;
public:

  Magazine();

  ~Magazine();

  void fire(const sf::Vector2f& begin, const float degree, const int32_t lifetime);

  void update(sf::Time time);

  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
extern Magazine* magazine;
