#include "main.hpp"

#include "Assets.hpp"
#include "Level.hpp"

int selectLevel() {

}

#ifdef DEBUG
int main() {
#else
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
#endif // DEBUG
  assets = new Assets;
  assets->load();

  LevelStorage storage;
  storage.loadTest();

  level = new Level;
  level->load(storage);

  sf::Event event;

  sf::RenderWindow window(sf::VideoMode(800, 600), "WarGame");
  window.setVerticalSyncEnabled(true);

  while(window.isOpen()) {
    while(window.pollEvent(event)) {
      switch(event.type) {

      }
    }

    window.clear();
    window.draw(*level);
    window.display();
  }
}