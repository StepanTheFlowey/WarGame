#pragma once

#include <vector>
#include "main.hpp"

class LevelInfo {
public:

  std::wstring name;
  std::wstring author;

  bool pack=false;
  std::vector<std::wstring> names;

  inline LevelInfo() {
    debug(L"LevelInfo()");
  }

  inline ~LevelInfo() {
    debug(L"~LevelInfo()");
  }

  void load(const std::wstring filename);
};