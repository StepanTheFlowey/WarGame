#include "Menu.hpp"

#include <utility>
#include <filesystem>
#include "Level.hpp"

int Menu::run() {
  gradient_[0] = sf::Vertex(
    sf::Vector2f(0.F, 0.F),
    sf::Color::Black
  );
  gradient_[1] = sf::Vertex(
    sf::Vector2f(0.F, context->videoMode.height),
    sf::Color::Black
  );
  gradient_[2] = sf::Vertex(
    sf::Vector2f(context->videoMode.width, context->videoMode.height),
    sf::Color::Black
  );
  gradient_[3] = sf::Vertex(
    sf::Vector2f(context->videoMode.width, 0.F),
    sf::Color::Black
  );

  sf::Text texts[] = {
    sf::Text("Continue", font, 32),
    sf::Text("New game", font, 32),
    sf::Text("Settings", font, 32),
    sf::Text("Exit", font, 32)
  };
  constexpr uint8_t textsCount = sizeof(texts) / sizeof(sf::Text);

  for(uint8_t i = 0; i < textsCount; ++i) {
    texts[i].setFillColor(sf::Color::White);
    texts[i].setOrigin(sf::Vector2f(texts[i].getGlobalBounds().width / 2, 0.F));
    texts[i].setPosition(sf::Vector2f(context->videoMode.width / 2, context->videoMode.height - 300 + i * 40));
  }

  uint8_t lastSelected = UINT8_MAX;
  while(context->alive()) {
    context->autoClock();

    if(gradientR_ < 0.5F) {
      gradientR_ += time.asSeconds() / 10.F;
    }
    else {
      gradientR_ = 0.5F;
    }
    if(gradientG_ < 0.25F) {
      gradientG_ += time.asSeconds() / 20.F;
    }
    else {
      gradientG_ = 0.25F;
    }
    if(gradientB_ > 1.0F) {
      gradientB_ -= time.asSeconds() / 5.F;
    }
    else {
      gradientB_ = 0.0F;
    }
    updateGradient(time);

    window.clear();
    window.draw(gradient_, 4, sf::Quads);
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

          gradient_[0].position = sf::Vector2f(0.F, 0.F);
          gradient_[1].position = sf::Vector2f(0.F, context->videoMode.height);
          gradient_[2].position = sf::Vector2f(context->videoMode.width, context->videoMode.height);
          gradient_[3].position = sf::Vector2f(context->videoMode.width, 0.F);
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

void Menu::updateGradient(const sf::Time time) {
  gradientTime_ -= time;
  if(gradientTime_ < sf::microseconds(0)) {
    gradientTime_ += sf::milliseconds(50);

    ++gradientDegree_;
    const float val = std::fabs(std::sinf(gradientDegree_ * F_DEG_TO_RAD)) * 255.F;

    const sf::Color color(
      static_cast<uint8_t>(val * gradientR_),
      static_cast<uint8_t>(val * gradientG_),
      static_cast<uint8_t>(val * gradientB_)
    );
    gradient_[1].color = color;
    gradient_[2].color = color;
  }
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

  std::vector<LevelInfo> levels;

  if(!fs::exists("maps")) {
    fs::create_directory("maps");
  }

  {
    std::wstring filename;
    for(auto& i : fs::directory_iterator("maps")) {
      if(!fs::is_directory(i)) {
        continue;
      }
      filename = i.path().wstring() + L"/level.dat";
      if(!fs::is_regular_file(filename)) {
        continue;
      }
      if(fs::is_empty(filename)) {
        continue;
      }

      levels.emplace_back();
      if(!levels.back().load(filename)) {
        levels.pop_back();
      }
    }
  }

  std::vector<std::pair<sf::Text, sf::Text>> texts;

  for(auto& i : levels) {
    size_t pos = texts.size();
    debug(pos);
    debug(i.name);
    debug(i.author);
    texts.push_back(std::make_pair(sf::Text(i.name, font, 16), sf::Text(i.author, font, 16)));
    auto& i = texts.back();
    i.first.setPosition(sf::Vector2f());
  }

  while(window.isOpen()) {
    context->autoClock();

    if(gradientR_ > 0.F) {
      gradientR_ -= time.asSeconds() / 20.F;
    }
    else {
      gradientR_ = 0.F;
    }
    if(gradientG_ > 0.1F) {
      gradientG_ -= time.asSeconds() / 40.F;
    }
    else {
      gradientG_ = 0.1F;
    }
    if(gradientB_ < 0.8F) {
      gradientB_ += time.asSeconds() / 5.F;
    }
    else {
      gradientB_ = 0.8F;
    }
    updateGradient(time);

    window.clear();
    window.draw(gradient_, 4, sf::Quads);
    for(auto& i : texts) {
      window.draw(i.first);
      window.draw(i.second);
    }
    window.display();

    while(context->pollEvent()) {
      switch(event.type) {
        case sf::Event::MouseMoved:

          break;
        case sf::Event::MouseButtonPressed:

          break;
      }
    }
  }
  return -1;
}

int Menu::settings() {
  return 0;
}
