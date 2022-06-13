#pragma once
#include <map>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "main.hpp"
#include "resource.h"
#include "Exeption.hpp"

class Context {
  std::map<uint16_t, sf::Font> font_;
  std::map<uint16_t, sf::Texture> texture_;
  std::map<uint16_t, sf::SoundBuffer> audio_;
  std::map<uint16_t, sf::Music> music_;
  sf::ContextSettings contextSettings_;
  sf::Context context_;
  bool fullscreen_ = false;
public:

  sf::RenderWindow window;
  sf::VideoMode videoMode;
  sf::Event event;

  sf::Clock clock;
  sf::Time time;

  Context();

  ~Context();

  void create();

  inline void destroy() {
    window.close();
  }

  inline bool alive() const {
    return window.isOpen();
  }

  void autoEvent();

  inline void autoClock() {
    time = clock.restart();
  }

  bool pollEvent();

  sf::Font* getFont(const uint16_t id);

  sf::Texture* getTexture(const uint16_t id);

  sf::SoundBuffer* getSound(const uint16_t id);

  sf::Music* getMusic(const uint16_t id);
};
extern Context* context;
