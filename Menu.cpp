#include "Menu.hpp"

#include <filesystem>
#include "Context.hpp"
#include "Level.hpp"

int Menu::run() {
  sf::RenderWindow& window = context->window;
  sf::Event& event = context->event;
  sf::Time& time = context->time;

  sf::Font& font = *context->getFont(ID_FON1);

  sf::Text texts[] = {
    sf::Text("Continue",font,32),
    sf::Text("New game",font,32),
    sf::Text("Settings",font,32),
    sf::Text("Exit",font,32)
  };
  constexpr uint8_t textsCount = sizeof(texts) / sizeof(sf::Text);

  for(uint8_t i = 0; i < textsCount; ++i) {
    texts[i].setFillColor(sf::Color::White);
    texts[i].setOrigin(sf::Vector2f(texts[i].getGlobalBounds().width / 2, 0.F));
    texts[i].setPosition(sf::Vector2f(context->videoMode.width / 2, context->videoMode.height - 300 + i * 40));
  }

  gradient[0] = sf::Vertex(
    sf::Vector2f(0.F, 0.F),
    sf::Color::Black
  );
  gradient[1] = sf::Vertex(
    sf::Vector2f(0.F, context->videoMode.height),
    sf::Color::Black
  );
  gradient[2] = sf::Vertex(
    sf::Vector2f(context->videoMode.width, context->videoMode.height),
    sf::Color::Black
  );
  gradient[3] = sf::Vertex(
    sf::Vector2f(context->videoMode.width, 0.F),
    sf::Color::Black
  );

  uint8_t lastSelected = UINT8_MAX;
  while(context->alive()) {
    time -= context->clock.restart();

    if(time < sf::microseconds(0)) {
      time += sf::milliseconds(50);

      ++gradientDegree;
      sf::Color color;
      color.r = static_cast<uint8_t>(std::abs(std::sinf(gradientDegree * F_DEG_TO_RAD)) * 128.F);
      color.g = color.r / 4;
      gradient[1].color = color;
      gradient[2].color = color;
    }

    window.clear();
    window.draw(gradient, 4, sf::Quads);
    for(uint8_t i = 0; i < textsCount; ++i) {
      window.draw(texts[i]);
    }
    window.display();

    while(context->pollEvent()) {
      switch(event.type) {
        case sf::Event::Resized:
          window.setView(sf::View(sf::FloatRect(0.F, 0.F, event.size.width, event.size.height)));

          for(uint8_t i = 0; i < textsCount; ++i) {
            texts[i].setPosition(sf::Vector2f(context->videoMode.width / 2, context->videoMode.height - 300 + i * 40));
          }

          gradient[0].position = sf::Vector2f(0.F, 0.F);
          gradient[1].position = sf::Vector2f(0.F, context->videoMode.height);
          gradient[2].position = sf::Vector2f(context->videoMode.width, context->videoMode.height);
          gradient[3].position = sf::Vector2f(context->videoMode.width, 0.F);
          break;
        case sf::Event::MouseMoved:
        {
          uint8_t selected = UINT8_MAX;
          for(uint8_t i = 0; i < textsCount; ++i) {
            if(texts[i].getGlobalBounds().contains(sf::Vector2f(event.mouseMove.x, event.mouseMove.y))) {
              texts[i].setFillColor(sf::Color::Cyan);
              selected = i;
            }
            else {
              if(i == lastSelected) {
                texts[i].setFillColor(sf::Color::White);
              }
            }
          }
          lastSelected = selected;
          break;
        }
        case sf::Event::MouseButtonPressed:
          if(event.mouseButton.button == sf::Mouse::Left) {
            int result = broker(lastSelected);
            if(result != -1) {
              return result;
            }
          }
          break;
      }
    }
  }
  return 0;
}

int Menu::broker(int button) {
  switch(button) {
    case 0:
      return 1;
    case 1:
    {
      int result = select();
      if(result != -1) {
        return result;
      }
      break;
    }
    case 2:
      settings();
      break;
    case 3:
      return 0;
  }
  return -1;
}

int Menu::select() {
  namespace fs = std::filesystem;

  std::vector<LevelInfo> levels_;
  
}

int Menu::settings() {
  return 0;
}
