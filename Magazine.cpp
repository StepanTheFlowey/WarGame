#include "Magazine.hpp"

#include "Entity.hpp"
#include "Level.hpp"

Magazine* magazine = nullptr;

Magazine::Magazine() {
  debug(L"Magazine()");
  
  bullets_.reserve(256);
}

Magazine::~Magazine() {
  debug(L"~Magazine()");

  for(auto i : bullets_) {
    delete i;
  }
}

void Magazine::fire(const sf::Vector2f& begin, const float degree, const int32_t lifetime) {
  bullets_.push_back(new Bullet(begin, degree, lifetime));
}

void Magazine::update(const sf::Time& time) {
  for(size_t i = 0; i < bullets_.size(); ++i) {
    Bullet* bullet = bullets_[i];
    bullet->update(time);
    if(bullet->expired()) {
      delete bullet;
      bullets_.erase(bullets_.begin() + i);
      --i;
      continue;
    }

    if(level->collide(bullet->getRect())) {
      delete bullet;
      bullets_.erase(bullets_.begin() + i);
      --i;
      continue;
    }

    Entity* entity = level->collideEntity(bullet->getRect());
    if(entity) {
      entity->damage(1);

      delete bullet;
      bullets_.erase(bullets_.begin() + i);
      --i;
      continue;
    }
  }
}

void Magazine::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  for(const auto& i : bullets_) {
    target.draw(*i);
  }
}
