#pragma once

#include <inttypes.h>
#include <type_traits>

enum class Direction : uint8_t {
  Null = 0,
  Forward = 1 << 0,
  Back = 1 << 1,
  Left = 1 << 2,
  Right = 1 << 3
};

inline Direction operator&(const Direction l, const Direction r) {
  using T = std::underlying_type_t<Direction>;
  return static_cast<Direction>(static_cast<T>(l) & static_cast<T>(r));
}

inline Direction& operator&=(Direction& l, const Direction r) {
  l = l & r;
  return l;
}

inline Direction operator|(const Direction l, const Direction r) {
  using T = std::underlying_type_t <Direction>;
  return static_cast<Direction>(static_cast<T>(l) | static_cast<T>(r));
}

inline Direction& operator|=(Direction& l, const Direction r) {
  l = l | r;
  return l;
}


inline Direction operator^(const Direction l, const Direction r) {
  using T = std::underlying_type_t <Direction>;
  return static_cast<Direction>(static_cast<T>(l) ^ static_cast<T>(r));
}

inline Direction& operator^=(Direction& l, const Direction r) {
  l = l ^ r;
  return l;
}