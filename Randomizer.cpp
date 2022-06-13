#include "Randomizer.hpp"

#include "main.hpp"

Randomizer* random = nullptr;

Randomizer::Randomizer() :
  random_generator(random_device()) {
  debug(L"Random(): " << random_generator());
}

