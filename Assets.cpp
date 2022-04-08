#include "Assets.hpp"

Assets* assets = nullptr;

void Assets::load() {
  font.loadFromFile("assets\\font\\fsys.ttf");
}

void Assets::save() const {

}
