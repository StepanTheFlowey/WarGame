#include "Menu.hpp"

#include <utility>
#include <filesystem>
#include "Level.hpp"

int Menu::run() {
  gradient_.setColor(sf::Vector3f(0.5F, 0.25F, 0.F), 0.1F);

  sf::Text texts[] = {
    sf::Text("Continue", font, 32),
    sf::Text("New game", font, 32),
    sf::Text("Settings", font, 32),
    sf::Text("Exit", font, 32)
  };
  constexpr uint8_t textsCount = sizeof(texts) / sizeof(sf::Text);

  for(uint8_t i = 0; i < textsCount; ++i) {
    texts[i].setFillColor(sf::Color::White);
    texts[i].setOrigin(sf::Vector2f(texts[i].getGlobalBounds().width / 2.F, 0.F));
    texts[i].setPosition(sf::Vector2f(
      static_cast<float>(context->videoMode.width / 2),
      static_cast<float>(context->videoMode.height - 300 + i * 40)
    ));
  }

  uint8_t lastSelected = UINT8_MAX;
  while(context->alive()) {
    context->autoClock();

    gradient_.update(time);

    window.clear();
    window.draw(gradient_);
    for(const auto& i : texts) {
      window.draw(i);
    }
    window.display();

    while(context->pollEvent()) {
      switch(event.type) {
        case sf::Event::Resized:
          window.setView(sf::View(sf::FloatRect(
            0.F, 0.F,
            static_cast<float>(event.size.width),
            static_cast<float>(event.size.height)
          )));

          gradient_.resize(sf::Vector2f(
            static_cast<float>(context->videoMode.width),
            static_cast<float>(context->videoMode.height)
          ));

          for(uint8_t i = 0; i < textsCount; ++i) {
            texts[i].setPosition(sf::Vector2f(
              static_cast<float>(context->videoMode.width / 2),
              static_cast<float>(context->videoMode.height - 300 + i * 40)
            ));
          }
          break;
        case sf::Event::MouseMoved:
        {
          if(lastSelected != UINT8_MAX) {
            texts[lastSelected].setFillColor(sf::Color::White);
          }

          uint8_t i;
          for(i = 0; i < textsCount; ++i) {
            if(texts[i].getGlobalBounds().contains(sf::Vector2f(
              static_cast<float>(event.mouseMove.x),
              static_cast<float>(event.mouseMove.y)
              ))) {
              texts[i].setFillColor(sf::Color::Cyan);
              break;
            }
          }
          lastSelected = (i == textsCount ? UINT8_MAX : i);
          break;
        }
        case sf::Event::MouseButtonPressed:
          if(event.mouseButton.button == sf::Mouse::Left) {
            int result = broker(lastSelected);
            if(result != -1) {
              return result;
            }
            gradient_.setColor(sf::Vector3f(0.5F, 0.25F, 0.F), 0.1F);
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

  std::vector<LevelInfo> levels;

  if(!fs::exists("maps")) {
    fs::create_directory("maps");
  }

  for(auto& i : fs::directory_iterator("maps")) {
    if(!fs::is_directory(i)) {
      continue;
    }
    std::wstring filename = i.path().wstring() + L"/level.dat";
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

  gradient_.setColor(sf::Vector3f(0.F, 0.1F, 0.8F), 0.1F);

  uint32_t page = 0;
  uint32_t pageSize = context->videoMode.height / 18 - 2;
  uint8_t lastSelected = UINT8_MAX;

  struct LevelTexts {
    sf::Text name;
    sf::Text author;
    sf::Text type;
    sf::IntRect hitbox;
  };
  std::vector<LevelTexts> texts;

  sf::Text back("Back", font, 32);
  back.setPosition(sf::Vector2f(10.F, context->videoMode.height - 40.F));

  size_t i = 0;
  while(window.isOpen()) {
    context->autoClock();

    if(i < levels.size()) {
      const auto& l = levels[i];

      texts.push_back(
        {
          sf::Text(l.name, font, 16),
          sf::Text(l.author, font, 16),
          sf::Text(l.pack ? "[PACK]" : "[SNGL]", font, 16),
          sf::IntRect(0, i * 18,context->videoMode.width, 18)
        });

      auto& t = texts.back();
      t.name.setPosition(sf::Vector2f(2, 2 + i * 18));
      t.author.setPosition(sf::Vector2f(context->videoMode.width - t.author.getGlobalBounds().width - 100, 2 + i * 18));
      t.type.setPosition(sf::Vector2f(context->videoMode.width - t.type.getGlobalBounds().width - 4, 2 + i * 18));
      ++i;
    }

    gradient_.update(time);

    window.clear();
    window.draw(gradient_);
    for(auto& i : texts) {
      window.draw(i.name);
      window.draw(i.author);
      window.draw(i.type);
    }
    window.draw(back);
    window.display();

    while(context->pollEvent()) {
      switch(event.type) {
        case sf::Event::Resized:
          window.setView(sf::View(sf::FloatRect(
            0.F, 0.F,
            static_cast<float>(event.size.width),
            static_cast<float>(event.size.height)
          )));

          gradient_.resize(sf::Vector2f(
            static_cast<float>(context->videoMode.width),
            static_cast<float>(context->videoMode.height)
          ));

          back.setPosition(sf::Vector2f(10.F, context->videoMode.height - 40.F));

          for(size_t i = 0; i < texts.size(); ++i) {
            auto& t = texts[i];
            t.name.setPosition(sf::Vector2f(2, 2 + i * 18));
            t.author.setPosition(sf::Vector2f(context->videoMode.width - t.author.getGlobalBounds().width - 100, 2 + i * 18));
            t.type.setPosition(sf::Vector2f(context->videoMode.width - t.type.getGlobalBounds().width - 4, 2 + i * 18));
          }
          break;
        case sf::Event::MouseMoved:
        {
          sf::Vector2f mouse(event.mouseMove.x, event.mouseMove.y);

          back.setFillColor(back.getGlobalBounds().contains(mouse) ? sf::Color::Cyan : sf::Color::White);

          if(lastSelected != UINT8_MAX) {
            texts[lastSelected].name.setFillColor(sf::Color::White);
            texts[lastSelected].author.setFillColor(sf::Color::White);
            texts[lastSelected].type.setFillColor(sf::Color::White);
          }

          uint8_t i;
          for(i = 0; i < texts.size(); ++i) {
            if(texts[i].hitbox.contains(sf::Vector2i(
              event.mouseMove.x,
              event.mouseMove.y
              ))) {
              texts[i].name.setFillColor(sf::Color::Cyan);
              texts[i].author.setFillColor(sf::Color::Cyan);
              texts[i].type.setFillColor(sf::Color::Cyan);
              break;
            }
          }
          lastSelected = (i == texts.size() ? UINT8_MAX : i);
        }
        break;
        case sf::Event::MouseButtonPressed:
          sf::Vector2f mouse(event.mouseButton.x, event.mouseButton.y);

          if(back.getGlobalBounds().contains(mouse)) {
            return -1;
          }
          break;
      }
    }
  }
  return -1;
}

int Menu::settings() {
  return 0;
}

Menu::Gradient::Gradient() {
  vt_[0] = sf::Vertex(
    sf::Vector2f(0.F, 0.F),
    sf::Color::Black
  );
  vt_[1] = sf::Vertex(
    sf::Vector2f(0.F, static_cast<float>(context->videoMode.height)),
    sf::Color::Black
  );
  vt_[2] = sf::Vertex(
    sf::Vector2f(static_cast<float>(context->videoMode.width), static_cast<float>(context->videoMode.height)),
    sf::Color::Black
  );
  vt_[3] = sf::Vertex(
    sf::Vector2f(static_cast<float>(context->videoMode.width), 0.F),
    sf::Color::Black
  );
}

void Menu::Gradient::setColor(const sf::Vector3f& color, const float speed) {
  speed_ = speed;

  target_ = color;
  prev_ = current_;
}

void Menu::Gradient::resize(const sf::Vector2f& size) {
  vt_[1].position = sf::Vector2f(0.F, size.y);
  vt_[2].position = size;
  vt_[3].position = sf::Vector2f(size.x, 0.F);
}

void Menu::Gradient::update(const sf::Time time) {
  time_ -= time;
  if(time_ < sf::microseconds(0)) {
    time_ = sf::milliseconds(50);

    if(target_ != current_) {
      current_ += (target_ - prev_) * speed_;
      if((target_.x < prev_.x && current_.x < target_.x)||
         (target_.x > prev_.x && current_.x > target_.x)) {
        current_.x = target_.x;
      }
      if((target_.y < prev_.y && current_.y < target_.y) ||
         (target_.y > prev_.y && current_.y > target_.y)) {
        current_.y = target_.y;
      }
      if((target_.z < prev_.z && current_.z < target_.z) ||
         (target_.z > prev_.z && current_.z > target_.z)) {
        current_.z = target_.z;
      }
    }

    ++degree_;
    const float val = std::fabs(std::sinf(degree_ * F_DEG_TO_RAD)) * 255.F;

    const sf::Color color(
      static_cast<uint8_t>(val * current_.x),
      static_cast<uint8_t>(val * current_.y),
      static_cast<uint8_t>(val * current_.z)
    );
    vt_[1].color = color;
    vt_[2].color = color;
  }
}

void Menu::Gradient::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(vt_, 4, sf::Quads);
}
