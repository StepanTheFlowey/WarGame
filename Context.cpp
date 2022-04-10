#include "Context.hpp"

#include <iostream>
#include <filesystem>
#include <Windows.h>
#include <Dwmapi.h>

namespace fs = std::filesystem;

Context* context = nullptr;

Context::Context() : window(), event() {
  debug(L"Context()");
  contextSettings_.antialiasingLevel = 0;
  contextSettings_.depthBits = 0;
  contextSettings_.majorVersion = 2;
  contextSettings_.minorVersion = 1;

  videoMode = sf::VideoMode(800, 600);
}

Context::~Context() {
  debug(L"~Context()");
  destroy();
}

void Context::load() {

}

void Context::save() const {

}

void Context::create() {
  if(fullscreen_) window.create(videoMode, L"WarGame", sf::Style::Fullscreen, contextSettings_);
  else            window.create(videoMode, L"WarGame", sf::Style::Default, contextSettings_);
  window.setVerticalSyncEnabled(true);
}

void Context::destroy() {
  window.close();
}

inline void Context::autoEvent() {
  if(window.pollEvent(event)) {
    if(sf::Event::Closed == event.type) {
      destroy();
    }
    else if(sf::Event::KeyPressed == event.type) {
      if(sf::Keyboard::F11 == event.key.code) {
        fullscreen_ = !fullscreen_;
        if(fullscreen_) videoMode = sf::VideoMode::getDesktopMode();
        else            videoMode = sf::VideoMode(800, 600);
      }
    }
  }
}

bool Context::pollEvent() {
  if(window.pollEvent(event)) {
    if(sf::Event::Closed == event.type) {
      window.close();
    }
    else if(sf::Event::KeyPressed == event.type) {
      if(sf::Keyboard::F11 == event.key.code) {
        fullscreen_ = !fullscreen_;
        if(fullscreen_) videoMode = sf::VideoMode::getDesktopMode();
        else            videoMode = sf::VideoMode(800, 600);
        create();
      }
      return true;
    }
    else {
      return true;
    }
  }
  return false;
}

sf::Font* Context::getFont(const uint16_t id) {
  const auto& iterator = font_.find(id);
  if(iterator != font_.end()) {
    return &iterator->second;
  }

  HRSRC hResource = FindResourceW(NULL, MAKEINTRESOURCEW(id), L"FON");
  if(!hResource) {
    exept(L"!hResource");
  }
  HGLOBAL hMemory = LoadResource(NULL, hResource);
  if(!hMemory) {
    exept(L"!hMemory");
  }

  font_[id];
  if(!font_[id].loadFromMemory(LockResource(hMemory), SizeofResource(NULL, hResource))) {
    exept(L"!texture_[id].loadFromMemory(...)");
  }
  return &font_[id];
}

sf::Texture* Context::getTexture(const uint16_t id) {
  const auto& iterator = texture_.find(id);
  if(iterator != texture_.end()) {
    return &iterator->second;
  }

  HRSRC hResource = FindResourceW(NULL, MAKEINTRESOURCEW(id), L"IMG");
  if(!hResource) {
    exept(L"!hResource");
  }
  HGLOBAL hMemory = LoadResource(NULL, hResource);
  if(!hMemory) {
    exept(L"!hMemory");
  }

  texture_[id];
  if(!texture_[id].loadFromMemory(LockResource(hMemory), SizeofResource(NULL, hResource))) {
    exept(L"!texture_[id].loadFromMemory(...)");
  }
  return &texture_[id];
}

sf::SoundBuffer* Context::getSound(const uint16_t id) {
  const auto& iterator = audio_.find(id);
  if(iterator != audio_.end()) {
    return &iterator->second;
  }

  HRSRC hResource = FindResourceW(NULL, MAKEINTRESOURCEW(id), L"SND");
  if(!hResource) {
    exept(L"!hResource");
  }
  HGLOBAL hMemory = LoadResource(NULL, hResource);
  if(!hMemory) {
    exept(L"!hMemory");
  }

  audio_[id];
  if(!audio_[id].loadFromMemory(LockResource(hMemory), SizeofResource(NULL, hResource))) {
    exept(L"!audio_[id].loadFromMemory(...)");
  }
  return &audio_[id];
}

sf::Music* Context::getMusic(const uint16_t id) {
  const auto& iterator = music_.find(id);
  if(iterator != music_.end()) {
    return &iterator->second;
  }

  HRSRC hResource = FindResourceW(NULL, MAKEINTRESOURCEW(id), L"MUS");
  if(!hResource) {
    exept(L"!hResource");
  }
  HGLOBAL hMemory = LoadResource(NULL, hResource);
  if(!hMemory) {
    exept(L"!hMemory");
  }

  music_[id];
  if(!music_[id].openFromMemory(LockResource(hMemory), SizeofResource(NULL, hResource))) {
    exept(L"!music_[id].openFromMemory(...)");
  }
  return &music_[id];
}
