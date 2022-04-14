#include "Game.hpp"

#include "Context.hpp"
#include "Magazine.hpp"
#include "Level.hpp"
#include "Player.hpp"
#include "Gui.hpp"

void Game::run() {
  sf::RenderWindow& window = context->window;
  sf::Event& event = context->event;
  sf::Time& time = context->time;
  sf::View gameView;
  sf::View uiView;

  context->create();
  uiView = window.getDefaultView();
  gameView.setSize(sf::Vector2f(context->videoMode.width, context->videoMode.height));
  while(context->alive()) {
    context->autoClock();

    magazine->update(time);
    level->update(time);
    player->update(time);

    gameView.setCenter(player->getPosition());

    window.clear(sf::Color::Magenta);
    window.setView(gameView);
    window.draw(*level);
    window.draw(*player);
    window.draw(*magazine);
    window.setView(uiView);
    window.draw(*gui);
    window.display();

    while(context->pollEvent()) {
      switch(event.type) {
        case sf::Event::Resized:
        {
          sf::Vector2f size(event.size.width, event.size.height);
          gameView.setSize(size);
          uiView.setSize(size);
          size.x /= 2.F;
          size.y /= 2.F;
          uiView.setCenter(size);
        }
        break;
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
              gameView.setSize(sf::Vector2f(context->videoMode.width, context->videoMode.height));
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
