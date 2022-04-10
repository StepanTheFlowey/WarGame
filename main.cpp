#include "main.hpp"

#include "Level.hpp"
#include "Context.hpp"
#include "Player.hpp"
#include "Gui.hpp"

int selectLevel() {
  return 0;
}

#ifdef DEBUG
int main() {
#else
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
#endif // DEBUG

  context = new Context;
  level = new Level;
  gui = new Gui;
  magazine = new Magazine;
  player = new Player;

  LevelStorage storage;
  storage.loadTest();
  level->load(storage);

  sf::RenderWindow& window = context->window;
  sf::Event& event = context->event;
  sf::Time& time = context->time;
  sf::View view;

  context->create();
  view.setSize(sf::Vector2f(context->videoMode.width, context->videoMode.height));
  while(context->alive()) {
    context->autoClock();

    magazine->update(time);
    level->update(time);
    player->update(time);

    view.setCenter(player->getPosition());

    window.setView(view);
    window.clear(sf::Color::Magenta);
    window.draw(*level);
    window.draw(*player);
    window.draw(*magazine);
    window.setView(window.getDefaultView());
    window.draw(*gui);
    window.display();

    while(context->pollEvent()) {
      switch(event.type) {
        case sf::Event::KeyPressed:
          switch(event.key.code) {
            case sf::Keyboard::Up:
            case sf::Keyboard::W:
              player->setWalk(Direction::Forward, true);
              break;
            case sf::Keyboard::Down:
            case sf::Keyboard::S:
              player->setWalk(Direction::Back, true);
              break;
            case sf::Keyboard::Left:
            case sf::Keyboard::A:
              player->setWalk(Direction::Left, true);
              break;
            case sf::Keyboard::Right:
            case sf::Keyboard::D:
              player->setWalk(Direction::Right, true);
              break;
            case sf::Keyboard::F11:
              view.setSize(sf::Vector2f(context->videoMode.width, context->videoMode.height));
              break;
            case sf::Keyboard::E:
              player->fire();
              break;
          }
          break;
        case sf::Event::KeyReleased:
          switch(event.key.code) {
            case sf::Keyboard::Up:
            case sf::Keyboard::W:
              player->setWalk(Direction::Forward, false);
              break;
            case sf::Keyboard::Down:
            case sf::Keyboard::S:
              player->setWalk(Direction::Back, false);
              break;
            case sf::Keyboard::Left:
            case sf::Keyboard::A:
              player->setWalk(Direction::Left, false);
              break;
            case sf::Keyboard::Right:
            case sf::Keyboard::D:
              player->setWalk(Direction::Right, false);
              break;
          }
          break;
      }
    }
  }
}