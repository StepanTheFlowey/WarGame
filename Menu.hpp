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

private:

  void updateGradient(const sf::Time time);

  int broker(int button);

  int select();

  int settings();

  sf::Time gradientTime_;
  sf::Vertex gradient_[4];
  uint16_t gradientDegree_ = 0;
  float gradientR_ = 0.0F;
  float gradientG_ = 0.0F;
  float gradientB_ = 0.0F;

  sf::RenderWindow& window = context->window;
  sf::Event& event = context->event;
  sf::Time& time = context->time;

  sf::Font& font = *context->getFont(ID_FON1);
};
