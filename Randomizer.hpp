#pragma once

#include <random>

class Randomizer {
  std::random_device random_device;
  std::mt19937 random_generator;
public:

  Randomizer();

  ~Randomizer() = default;

  inline unsigned int get() {
    return random_generator();
  }
};
extern Randomizer* random;
