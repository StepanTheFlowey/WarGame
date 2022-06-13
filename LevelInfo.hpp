#pragma once

#include <vector>
#include "main.hpp"

class Level;

class LevelInfo {
  std::string filename_;
public:

  std::wstring name;
  std::wstring author;

  bool pack = false;
  std::vector<std::wstring> levels;

  inline LevelInfo() {
    debug(L"LevelInfo()");
  }

  inline ~LevelInfo() {
    debug(L"~LevelInfo()");
  }

  bool load(const std::string filename);

  friend Level;
};