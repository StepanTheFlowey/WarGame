#pragma once

#include "main.hpp"
#include "Context.hpp"
#include <SFML/Graphics/Vertex.hpp>

class Menu {
public:

  inline Menu() {
    debug(L"Menu()");
  }

  inline ~Menu() {
    debug(L"~Menu()");
  }

  int run();

protected:

  int broker(int button);

  int select();

  int settings();

  class Gradient : public sf::Drawable {
  public:

    Gradient();

    ~Gradient() = default;

    void setColor(const sf::Vector3f& color, const float speed);

    void resize(const sf::Vector2f& size);

    void update(const sf::Time time);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
  protected:

    sf::Vector3f current_;
    sf::Vector3f target_;
    sf::Vector3f prev_;
    float speed_;

    sf::Vertex vt_[4];
    sf::Time time_;
    uint16_t degree_ = 0;
  } gradient_;

  sf::RenderWindow& window = context->window;
  sf::Event& event = context->event;
  sf::Time& time = context->time;

  sf::Font& font = *context->getFont(ID_FON1);
};
