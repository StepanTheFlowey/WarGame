#pragma once

#include <vector>
#include "main.hpp"

class LevelInfo {
  std::wstring filename_;
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

  bool load(const std::wstring filename);
};