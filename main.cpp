#define WIN32_LEAN_AND_MEAN
#include <cmath>
#include <iostream>
#include <fstream>
#include <Windows.h>

#include <rapidjson/document.h>
#include <rapidjson/error/en.h>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "types.hpp"
#include "resources.hpp"
#include "level.hpp"

bool playerDied = false;
bool playerWin = false;

uint16_t ScrW = sf::VideoMode::getDesktopMode().width;
uint16_t ScrH = sf::VideoMode::getDesktopMode().height;

sf::Font* fsys;

enum Diffucult {
  Easy,
  Normal,
  Hard
};

enum class Direction {
  None,
  Up,
  Right,
  Down,
  Left
};

struct TextureSet {
  sf::Texture up[4];
  sf::Texture down[4];
  sf::Texture left[2];
  sf::Texture right[2];
  sf::Texture death[15];
};
TextureSet* txset;

struct Chunk {
  sf::RectangleShape rect[8][8];
  bool isWall[8][8];
  sf::IntRect chunkrect;
};

#ifdef DEBUG
inline void debug(std::wstring str) {
  std::wcout << str << std::endl;
}
#endif // DEBUG

class Bullet : public sf::Drawable {
public:
  float offsetX = 0.0F;
  float offsetY = 0.0F;
  enum class Type {
    None,
    Normal,
    Big,
    Small,
    Fire,
    Blast
  };
  bool work = false;
  sf::CircleShape circle;

  Bullet() {
#ifdef DEBUG
    debug(L"Bullet init");
#endif // DEBUG
    circle.setPointCount(12);
  }

  void setType(Type type) {
    switch(type) {
      case Type::Normal:
        circle.setRadius(5);
        circle.setFillColor(sf::Color(32, 32, 32));
        break;
      case Type::Big:
        circle.setRadius(7);
        circle.setFillColor(sf::Color(32, 32, 32));
        break;
      case Type::Small:
        circle.setRadius(3);
        circle.setFillColor(sf::Color(32, 32, 32));
        break;
      case Type::Fire:
        circle.setRadius(5);
        circle.setFillColor(sf::Color(64, 32, 32));
        break;
      case Type::Blast:
        circle.setRadius(7);
        circle.setFillColor(sf::Color(64, 32, 32));
        break;
    }
  }

  void fire(sf::Vector2f v, float offsX, float offsY) {
    work = true;
    offsetX = offsX;
    offsetY = offsY;
    circle.setPosition(v);
  }

  void update() {
    if(work) {
      circle.move(offsetX, offsetY);
      if(!sf::FloatRect(0, 0, 4096, 4096).contains(circle.getPosition())) {
        work = false;
      }
    }
  }

  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if(work) {
      target.draw(circle);
    }
  }
};

Chunk chunk[8][8];
class Player : public sf::Drawable {
  float frame = 0;
  Direction lastd = Direction::None;
  bool moved = false;
  std::vector<Bullet> bullet;
  short stack = 0;
  bool died = false;
  float dieAnim = 0;
public:
  bool cango = false;
  float health = 20;
  sf::RectangleShape healthrect, healthbar;
  sf::Sprite spr;
  Player() {
#ifdef DEBUG
    debug(L"Player init started");
#endif // DEBUG
    bullet.resize(32);
    for(uint8_t i = 0; i < bullet.size(); i++) {
      bullet[i].setType(Bullet::Type::Normal);
    }
    healthrect.setFillColor(sf::Color(0, 0, 0, 0));
    healthrect.setPosition(10, 10);
    healthrect.setOutlineThickness(5);
    healthrect.setOutlineColor(sf::Color::Black);
    healthrect.setSize(sf::Vector2f(200, 30));
    healthbar.setFillColor(sf::Color(255, 32, 32));
    healthbar.setPosition(10, 10);
    healthbar.setSize(sf::Vector2f(200, 30));
#ifdef DEBUG
    debug(L"Player init done");
#endif // DEBUG
  }

  bool checkWall() {
    bool cango = false;
    if(!sf::FloatRect(0, 0, 4096, 4096).intersects(spr.getGlobalBounds())) {
      playerWin = true;
      return false;
    }
    for(uint8_t i = 0; i < 8; i++) {
      for(uint8_t j = 0; j < 8; j++) {
        if(sf::IntRect(spr.getPosition().x - ScrW / 2, spr.getPosition().y - ScrH / 2, ScrW, ScrH).intersects(chunk[i][j].chunkrect)) {
          for(uint8_t k = 0; k < 8; k++) {
            for(uint8_t l = 0; l < 8; l++) {
              if(spr.getGlobalBounds().intersects(chunk[i][j].rect[k][l].getGlobalBounds()) && chunk[i][j].isWall[k][l]) {
                cango = true;
              }
            }
          }
        }
      }
    }
    return cango;
  }

  void damage(float amount) {
    if(health > 0) {
      health += -amount;
      if(health <= 0) {
        playerDied = true;
      }
    }
    else {
      playerDied = true;
    }
  }

  void move(uint16_t offset, Direction d) {
    if(!playerDied) {
      sf::Vector2f viewcenter = spr.getPosition();
      moved = true;
      lastd = d;
      frame += 0.2;
      switch(d) {
        case Direction::Up:
        {
          if(frame > 3.9F) {
            frame = 0;
          }
          spr.move(0, -offset);
          if(checkWall()) {
            spr.move(0, offset);
          }
          spr.setTexture(txset->up[(int)frame]);
          break;
        }
        case Direction::Right:
        {
          if(frame > 1.9F) {
            frame = 0;
          }
          spr.move(offset, 0);
          if(checkWall()) {
            spr.move(-offset, 0);
          }
          spr.setTexture(txset->right[(int)frame]);
          break;
        }
        case Direction::Down:
        {
          if(frame > 3.9F) {
            frame = 0;
          }
          spr.move(0, offset);
          if(checkWall()) {
            spr.move(0, -offset);
          }
          spr.setTexture(txset->down[(int)frame]);
          break;
        }
        case Direction::Left:
        {
          if(frame > 1.9F) {
            frame = 0;
          }
          spr.move(-offset, 0);
          if(checkWall()) {
            spr.move(offset, 0);
          }
          spr.setTexture(txset->left[(int)frame]);
          break;
        }
      }
    }
  }

  int getBulletSize() {
    return bullet.size();
  }

  sf::FloatRect getBulletRect(uint16_t bulletNum) {
    return bullet[bulletNum].circle.getGlobalBounds();
  }

  //Remove bullet
  inline void killBullet(uint16_t bulletNum) {
    bullet[bulletNum].work = false;
    bullet[bulletNum].circle.setPosition(sf::Vector2f(-10, -10));
  }

  //Fire function
  void fire() {
    if(!playerDied) {//We cannot fire if we died
      switch(lastd) {//Bullet fire at last pos
        case Direction::Up:
          bullet[stack].fire(spr.getPosition(), 0, -7);
          break;
        case Direction::Right:
          bullet[stack].fire(spr.getPosition(), 7, 0);
          break;
        case Direction::Down:
          bullet[stack].fire(spr.getPosition(), 0, 7);
          break;
        case Direction::Left:
          bullet[stack].fire(spr.getPosition(), -7, 0);
          break;
      }
      //Bullet counter
      stack++;
      if(stack == bullet.size()) {
        stack = 0;
      }
    }
  }

  void update() {
    //Bullet calc
    for(uint8_t i = 0; i < bullet.size(); i++) {
      bool cango = false;
      for(uint8_t j = 0; j < 8; j++) {
        for(uint8_t k = 0; k < 8; k++) {
          if(sf::IntRect(spr.getPosition().x - ScrW / 2, spr.getPosition().y - ScrH / 2, ScrW, ScrH).intersects(chunk[j][k].chunkrect)) {
            for(uint8_t l = 0; l < 8; l++) {
              for(uint8_t m = 0; m < 8; m++) {
                if(chunk[j][k].rect[l][m].getGlobalBounds().contains(bullet[i].circle.getPosition()) && chunk[j][k].isWall[l][m]) {
                  cango = true;
                }
              }
            }
          }
        }
      }
      if(cango) {
        killBullet(i);
      }
    }
    //Set origin (Why?)
    spr.setOrigin(
      spr.getGlobalBounds().width / 20,
      spr.getGlobalBounds().height / 20
    );
    //Set health bar size
    healthbar.setSize(sf::Vector2f(health * 10, 30));
    //Set stay sprite if player stopped move
    if(moved) {
      moved = false;
    }
    else {
      switch(lastd) {
        case Direction::Up:
        {
          spr.setTexture(txset->up[0]);
          break;
        }
        case Direction::Right:
        {
          spr.setTexture(txset->right[0]);
          break;
        }
        case Direction::Down:
        {
          spr.setTexture(txset->down[0]);
          break;
        }
        case Direction::Left:
        {
          spr.setTexture(txset->left[0]);
          break;
        }
      }
    }
    //Death animation
    if(playerDied) {
      spr.setTexture(txset->death[(int32_t)dieAnim]);   //Set sprite
      if(dieAnim < 14) {                             //Sprite counter security
        dieAnim += 0.1;                              //Sprite counter
      }
    }
    for(auto& i : bullet) {
      i.update();
    }
  }

  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(spr);     //Drawing player sprite
    for(auto& i : bullet) { //Bullet counter
      target.draw(i);     //Bullet draw
    }
  }

  void drawUI(sf::RenderWindow& window) {
    window.draw(healthrect);
    window.draw(healthbar);
  }
};
//Player object
Player pl;
//Enemy class
class Enemy : public sf::Drawable {
  float frame = 0;
  Direction lastd = Direction::None;
  bool moved = false;
  std::vector<Bullet> bullet;
  uint16_t stack = 0;
  uint8_t health = 3;
  bool died = false;
  sf::FloatRect rect[4];
  sf::Clock fireclock;
#ifdef DEBUG
  sf::RectangleShape debugRect[4];
#endif
  float dieAnim = 0;
public:
  sf::RectangleShape healthrect, healthbar;
  sf::Sprite spr;
  Enemy() {
#ifdef DEBUG
    debug(L"Enemy init");
#endif // DEBUG
    bullet.resize(32);
    for(auto& i : bullet) {
      i.setType(Bullet::Type::Normal);
    }
    spr.setTexture(txset->down[0]);
    healthrect.setFillColor(sf::Color(0, 0, 0));
    int32_t x = spr.getGlobalBounds().left + 4;
    int32_t y = spr.getGlobalBounds().top - 20;
    healthrect.setPosition(x, y);
    healthrect.setOutlineThickness(2);
    healthrect.setOutlineColor(sf::Color::Black);
    healthrect.setSize(sf::Vector2f(30, 5));
    healthbar.setFillColor(sf::Color(255, 32, 32));
    healthbar.setPosition(x, y);
    healthbar.setSize(sf::Vector2f(30, 5));
    spr.setScale(sf::Vector2f(5, 5));
    spr.setColor(sf::Color(255, 0, 0));
    move(0, Direction::Down);
    rect[0] = sf::FloatRect(0, 0, 10, 400);
    rect[1] = sf::FloatRect(0, 0, 400, 10);
    rect[2] = sf::FloatRect(0, 0, 10, 400);
    rect[3] = sf::FloatRect(0, 0, 400, 10);
#ifdef DEBUG
    for(uint8_t i = 0; i < 4; i++) {
      debugRect[i].setFillColor(sf::Color(0, 0, 255, 100));
    }
    debug(L"Enemy init done");
#endif // DEBUG
  }

  bool checkWall() {
    bool cango = false;
    if(!sf::FloatRect(0, 0, 4096, 4096).intersects(spr.getGlobalBounds())) {
      health += -0.5;
      return true;
    }
    for(uint8_t i = 0; i < 8; i++) {
      for(uint8_t j = 0; j < 8; j++) {
        if(sf::IntRect(spr.getPosition().x - ScrW / 2, spr.getPosition().y - ScrH / 2, ScrW, ScrH).intersects(chunk[i][j].chunkrect)) {
          for(uint8_t k = 0; k < 8; k++) {
            for(uint8_t l = 0; l < 8; l++) {
              if(spr.getGlobalBounds().intersects(chunk[i][j].rect[k][l].getGlobalBounds()) && chunk[i][j].isWall[k][l]) {
                cango = true;
              }
            }
          }
        }
      }
    }
    return cango;
  }

  inline void killBullet(std::size_t bulletNum) {
    bullet[bulletNum].work = false;
    bullet[bulletNum].circle.setPosition(sf::Vector2f(-10, -10));
  }

  bool damage(uint16_t amount) {
    if(!died) {
      if(health <= amount) {
        health = 0;
        died = true;
      }
      else {
        health += -amount;
      }
      return true;
    }
    else {
      return false;
    }
  }

  void move(int offset, Direction d) {
    moved = true;
    lastd = d;
    frame += 0.1;
    switch(d) {
      case Direction::Up:
      {
        if(frame > 3.9F) {
          frame = 0;
        }
        spr.move(0, -offset);
        spr.setTexture(txset->up[(int)frame]);
        break;
      }
      case Direction::Right:
      {
        if(frame > 1.9F) {
          frame = 0;
        }
        spr.move(offset, 0);
        spr.setTexture(txset->right[(int)frame]);
        break;
      }
      case Direction::Down:
      {
        if(frame > 3.9F) {
          frame = 0;
        }
        spr.move(0, offset);
        spr.setTexture(txset->down[(int)frame]);
        break;
      }
      case Direction::Left:
      {
        if(frame > 1.9F) {
          frame = 0;
        }
        spr.move(-offset, 0);
        spr.setTexture(txset->left[(int)frame]);
        break;
      }
    }
    if(moved) {
      moved = false;
    }
    else {
      switch(lastd) {
        case Direction::Up:
        {
          spr.setTexture(txset->up[0]);
          break;
        }
        case Direction::Right:
        {
          spr.setTexture(txset->right[0]);
          break;
        }
        case Direction::Down:
        {
          spr.setTexture(txset->down[0]);
          break;
        }
        case Direction::Left:
        {
          spr.setTexture(txset->left[0]);
          break;
        }
      }
    }
  }

  void fire() {
    if(fireclock.getElapsedTime() > sf::seconds(0.5) && !died) {
      fireclock.restart();
      switch(lastd) {
        case Direction::Up:
          bullet[stack].fire(spr.getPosition(), 0, -7);
          break;
        case Direction::Right:
          bullet[stack].fire(spr.getPosition(), 7, 0);
          break;
        case Direction::Down:
          bullet[stack].fire(spr.getPosition(), 0, 7);
          break;
        case Direction::Left:
          bullet[stack].fire(spr.getPosition(), -7, 0);
          break;
      }
      stack++;
      if(stack == bullet.size()) {
        stack = 0;
      }
    }
  }

  void update() {
    //Bullet calc
    for(uint8_t i = 0; i < bullet.size(); i++) {
      bool cango = false;
      for(uint8_t i = 0; i < 8; i++) {
        for(uint8_t j = 0; j < 8; j++) {
          if(sf::IntRect(spr.getPosition().x - ScrW / 2, spr.getPosition().y - ScrH / 2, ScrW, ScrH).intersects(chunk[i][j].chunkrect)) {
            for(uint8_t k = 0; k < 8; k++) {
              for(uint8_t l = 0; l < 8; l++) {
                if(chunk[i][j].rect[k][l].getGlobalBounds().contains(bullet[i].circle.getPosition()) && chunk[i][j].isWall[k][l]) {
                  cango = true;
                }
              }
            }
          }
        }
      }
      if(cango) {
        killBullet(i);
      }
    }
    rect[0].left = spr.getPosition().x + 5;
    rect[0].top = spr.getPosition().y + 20;
    rect[1].left = spr.getPosition().x + 20;
    rect[1].top = spr.getPosition().y + 5;
    rect[2].left = spr.getPosition().x + 5;
    rect[2].top = spr.getPosition().y - 400;
    rect[3].left = spr.getPosition().x - 400;
    rect[3].top = spr.getPosition().y + 5;
    for(uint8_t i = 0; i < 4; i++) {
#ifdef DEBUG
      debugRect[i].setPosition(rect[i].left, rect[i].top);
      debugRect[i].setSize(sf::Vector2f(rect[i].width, rect[i].height));
#endif // DEBUG
      if(rect[i].intersects(pl.spr.getGlobalBounds())) {
        switch(i) {
          case 0:
          {
            move(0, Direction::Down);
            fire();
            break;
          }
          case 1:
          {
            move(0, Direction::Right);
            fire();
            break;
          }
          case 2:
          {
            move(0, Direction::Up);
            fire();
            break;
          }
          case 3:
          {
            move(0, Direction::Left);
            fire();
            break;
          }
        }
      }
    }
    for(uint8_t i = 0; i < bullet.size(); i++) {
      if(bullet[i].circle.getGlobalBounds().intersects(pl.spr.getGlobalBounds())) {
        pl.damage(1);
        killBullet(i);
      }
    }
    spr.setOrigin(
      spr.getGlobalBounds().width / 20,
      spr.getGlobalBounds().height / 20
    );
    healthbar.setSize(sf::Vector2f(health * 10, 5));
    int x = spr.getGlobalBounds().left + 4;
    int y = spr.getGlobalBounds().top - 20;
    healthrect.setPosition(x, y);
    healthbar.setPosition(x, y);
    if(died) {
      spr.setTexture(txset->death[(int)dieAnim]);   //Set sprite
      if(dieAnim < 14) {                             //Sprite counter security
        dieAnim += 0.1;                              //Sprite counter
      }
    }
    for(auto& i : bullet) {
      i.update();
    }
  }

  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(spr);
    for(auto& i : bullet) {
      target.draw(i);
    }
    if(!died) {
#ifdef DEBUG
      for(uint8_t i = 0; i < 4; i++) {
        target.draw(debugRect[i]);
      }
#endif // DEBUG
      target.draw(healthrect);
      target.draw(healthbar);
    }
  }
};

struct MapContainer {
  uint8_t map[64][64];
};
std::vector<MapContainer> mapsContainer;
uint32_t mapNum = 0;
//Map class
class Map {
  //Enemyes
  std::vector<Enemy> enemy;
public:
  //Constructor
  Map() {
#ifdef DEBUG
    debug(L"Map init started");
#endif // DEBUG
    //Map tiles initialization
    for(uint8_t i = 0; i < 8; i++) {
      for(uint8_t j = 0; j < 8; j++) {
        chunk[i][j].chunkrect = sf::IntRect(512 * i, 512 * j, 512, 512);
        for(uint8_t k = 0; k < 8; k++) {
          for(uint8_t l = 0; l < 8; l++) {
            chunk[i][j].rect[k][l].setPosition(i * 512 + k * 64, j * 512 + l * 64);
            chunk[i][j].rect[k][l].setSize(sf::Vector2f(64, 64));
            chunk[i][j].rect[k][l].setFillColor(sf::Color(rand() % 255, rand() % 255, rand() % 255));
          }
        }
      }
    }
#ifdef DEBUG
    debug(L"Map init done");
#endif // DEBUG
  }
  void loadMap(MapContainer map) {
    //Load map from array to tiles
    for(uint8_t i = 0; i < 64; i++) {
      for(uint8_t j = 0; j < 64; j++) {
        switch(map.map[j][i]) {
          //Empty tile
          case 0:
          {
            chunk[i / 8][j / 8].rect[i - i / 8 * 8][j - j / 8 * 8].setFillColor(sf::Color::White);
            chunk[i / 8][j / 8].isWall[i - i / 8 * 8][j - j / 8 * 8] = false;
            break;
          }
          //Wall
          case 1:
          {
            chunk[i / 8][j / 8].rect[i - i / 8 * 8][j - j / 8 * 8].setFillColor(sf::Color::Black);
            chunk[i / 8][j / 8].isWall[i - i / 8 * 8][j - j / 8 * 8] = true;
            break;
          }
          //Enemy
          case 2:
          {
            chunk[i / 8][j / 8].rect[i - i / 8 * 8][j - j / 8 * 8].setFillColor(sf::Color::White);
            int x = chunk[i / 8][j / 8].rect[i - i / 8 * 8][j - j / 8 * 8].getPosition().x + 32;
            int y = chunk[i / 8][j / 8].rect[i - i / 8 * 8][j - j / 8 * 8].getPosition().y + 32;
            enemy.push_back(Enemy());
            enemy[enemy.size() - 1].spr.setPosition(x, y);
            chunk[i / 8][j / 8].isWall[i - i / 8 * 8][j - j / 8 * 8] = false;
            break;
          }
          //Player spawnpoint
          case 3:
          {
            chunk[i / 8][j / 8].rect[i - i / 8 * 8][j - j / 8 * 8].setFillColor(sf::Color::White);
            int x = chunk[i / 8][j / 8].rect[i - i / 8 * 8][j - j / 8 * 8].getPosition().x + 32;
            int y = chunk[i / 8][j / 8].rect[i - i / 8 * 8][j - j / 8 * 8].getPosition().y + 32;
            pl.spr.setPosition(x, y);
            chunk[i / 8][j / 8].isWall[i - i / 8 * 8][j - j / 8 * 8] = false;
            break;
          }
        }
      }
    }
  }

  void update() {
    //Player bullet check
    for(uint8_t i = 0; i < enemy.size(); i++) {
      for(uint8_t j = 0; j < pl.getBulletSize(); j++) {
        if(enemy[i].spr.getGlobalBounds().intersects(pl.getBulletRect(j))) {
          if(enemy[i].damage(1)) {
            pl.killBullet(j);
          }
        }
      }
    }
    for(auto& i : enemy) {
      i.update();
    }
  }

  void draw(sf::RenderWindow& window, sf::Vector2f viewcenter) {
    //Draw visible chunks
    for(uint8_t i = 0; i < 8; i++) {
      for(uint8_t j = 0; j < 8; j++) {
        if(sf::IntRect(viewcenter.x - ScrW / 2, viewcenter.y - ScrH / 2, ScrW, ScrH).intersects(chunk[i][j].chunkrect)) {
          for(uint8_t k = 0; k < 8; k++) {
            for(uint8_t l = 0; l < 8; l++) {
              window.draw(chunk[i][j].rect[k][l]);
            }
          }
        }
      }
    }
    //Draw enemy
    for(auto& i : enemy) {
      window.draw(i);
    }
  }
};

//Work variable
bool load = true;
//Load screen function
void loadScreen() {
  sf::RectangleShape rect[8];
  for(uint8_t i = 0; i < 8; i++) {
    rect[i].setSize(sf::Vector2f(20, 20));
    rect[i].setOrigin(sf::Vector2f(10, 10));
    rect[i].setFillColor(sf::Color::White);
  }
  uint16_t pos[8] = {0,45,90,135,180,225,270,315};
  sf::RenderWindow window(sf::VideoMode(200, 200), "Loading...", sf::Style::None);
  window.setFramerateLimit(60);
  sf::Event event;

  float rad = 0.0F;
  while(window.isOpen() && load) {
    while(window.pollEvent(event)) {
      if(event.type == sf::Event::Closed) {
        window.close();
      }
    }
    window.clear(sf::Color::Green);
    for(uint8_t i = 0; i < 8; i++) {
      pos[i]++;
      rad = pos[i] * 3.141592653589793238463 / 180;
      rect[i].setPosition(sf::Vector2f(sin(rad) * 50 + 100, cos(rad) * 50 + 100));
      rect[i].setFillColor(sf::Color(sin(rad) * 127 + 128, 0, cos(rad) * 127 + 128));
      window.draw(rect[i]);
    }
    window.display();
  }
  window.close();
}

void selectmap(sf::RenderWindow& window) {
#ifdef DEBUG
  debug(L"Map file find started");
#endif // DEBUG
  std::vector<std::vector<MapContainer>> maps;
  std::vector<std::wstring> filelist;
  {
    WIN32_FIND_DATAW ffd;
    HANDLE hFind;
    hFind = FindFirstFileW(L"maps\\*", &ffd);
    if(hFind == INVALID_HANDLE_VALUE) {
      exit(1);
    }
    FindNextFileW(hFind, &ffd);
    while(FindNextFileW(hFind, &ffd) != 0) {
      if(!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
#ifdef DEBUG
        debug(L"Found file in \'maps\' dir: " + (std::wstring)ffd.cFileName);
#endif // DEBUG
        filelist.push_back(ffd.cFileName);
      }
      else {
#ifdef DEBUG
        debug(L"Skipped found: " + (std::wstring)ffd.cFileName);
#endif // DEBUG
      }
    }
    FindClose(hFind);
  }
#ifdef DEBUG
  debug(L"Map file find closed");
#endif // DEBUG
  std::vector<std::string> maplist;
  std::ifstream file;
  std::string buff, content;
  for(uint16_t i = 0; i < filelist.size(); i++) {
#ifdef DEBUG
    debug(L"Opening file \'" + filelist[i] + L"\'");
#endif // DEBUG
    file.open(L"maps\\" + filelist[i]);
    if(!file) {
#ifdef DEBUG
      debug(L"Cannot open file \'" + filelist[i] + L"\'\nFile skipped");
#endif // DEBUG
      break;
    }
#ifdef DEBUG
    debug(L"Getting file content");
#endif // DEBUG
    content.clear();
    while(!file.eof()) {
      buff.clear();
      getline(file, buff);
      content += buff;
    }
    file.close();
    //debug("File closed\nStarting file content parse");
    rapidjson::Document d;
    d.Parse(content.c_str());
    //debug("Done file content parsing");
    if(d.HasParseError()) {
      //debug("Error parsing file content\nFile skipped");
      break;
    }
    if(!d.HasMember("type")) {
      //debug("Error, file has no member \'type\'\nFile skipped");
      break;
    }
    if(!d["type"].IsInt()) {
      //debug("Error, member \'type\' is not integer\nFile skipped");
      break;
    }
    if(d["type"].GetInt() == 0) {
      //debug("Error, member \'type\' is zero\nNote: To mark you file as map give that member value 1, to mark as map pack give member value 2\nFile skipped");
      break;
    }
    if(!d.HasMember("name")) {
      //debug("Error, file has no member \'name\'\nFile skipped");
      break;
    }
    if(!d["name"].IsString()) {
      //debug("Error, member \'name\' is not string\nFile skipped");
      break;
    }
    if(d["type"].GetInt() == 1) {
      if(!d.HasMember("map")) {
        //debug("Error, file has no member \'map\'\nFile skipped");
        break;
      }
      if(!d["map"].IsArray()) {
        //debug("Error, member \'map\' is not array\nFile skipped");
        break;
      }
      //debug("Preparing map container");
      maplist.push_back(d["name"].GetString() + (std::string)" [MAP]");
      maps.push_back(std::vector<MapContainer>(1));
      for(rapidjson::SizeType i = 0; i < d["map"].Size(); i++) {
        for(rapidjson::SizeType j = 0; j < d["map"][i].Size(); j++) {
          maps[maps.size() - 1][0].map[i][j] = d["map"][i][j].GetInt();
        }
      }
    }
  }
  std::vector<sf::Text> textline(maplist.size());
  for(uint16_t i = 0; i < textline.size(); i++) {
    textline[i].setFont(*fsys);
    textline[i].setString(maplist[i]);
    textline[i].setFillColor(sf::Color::White);
    textline[i].setPosition(sf::Vector2f(ScrW / 2 - 240, ScrH / 2 - 150 + i * 30));
  }
  //Gradient
  std::vector<sf::RectangleShape> gradient(ScrH);
  for(uint16_t i = 0; i < gradient.size(); i++) {
    gradient[i].setSize(sf::Vector2f(ScrW, 1));
    gradient[i].setPosition(sf::Vector2f(0, i));
    gradient[i].setFillColor(sf::Color((float)i / gradient.size() * 100, 0, (float)i / gradient.size() * 100));
  }
  //Rect
  sf::RectangleShape rect;
  rect.setSize(sf::Vector2f(500, 300));
  rect.setOrigin(250, 150);
  rect.setPosition(sf::Vector2f(ScrW / 2, ScrH / 2));
  rect.setFillColor(sf::Color(50, 50, 50, 100));
  bool work = true;
  bool selected = false;
  float deg = 0;
  float rad = 0;
  sf::Event event;
  //Window loop
  int16_t mapnum = -1;
  while(window.isOpen() && work) {
    while(window.pollEvent(event)) {
      if(event.type == sf::Event::Closed) {
        window.close();
        exit(0);
      }
    }
    //Covert degree to radian
    rad = deg * 3.141592653589793238463 / 180;
    //Calculate gradient
    for(int i = 0; i < gradient.size(); i++) {
      gradient[i].setFillColor(sf::Color((float)i / gradient.size() * abs(sin(rad) * 80), 0, (float)i / gradient.size() * abs(sin(rad) * 80)));
    }
    //Adding degree
    deg += 0.2;
    if(deg > 360) {
      deg = 0;
    }

    if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && mapnum != -1) {
      work = false;
    }
    //Drawing
    window.clear(sf::Color::Black);
    for(uint16_t i = 0; i < gradient.size(); i++) {
      window.draw(gradient[i]);
    }
    window.draw(rect);
    selected = false;
    for(uint16_t i = 0; i < textline.size(); i++) {
      if(textline[i].getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))) {
        textline[i].setFillColor(sf::Color::Yellow);
        mapnum = i;
        selected = true;
      }
      else {
        textline[i].setFillColor(sf::Color::White);
      }
      window.draw(textline[i]);
    }
    if(!selected) {
      mapnum = -1;
    }
    window.display();
  }
  mapsContainer = maps[mapnum];
}

int main() {
#ifndef DEBUG
  FreeConsole();
#else // !DEBUG
  CONSOLE_FONT_INFOEX cfi;
  cfi.cbSize = sizeof(cfi);
  cfi.nFont = 0;
  cfi.dwFontSize.X = 8;
  cfi.dwFontSize.Y = 12;
  cfi.FontFamily = FF_DONTCARE;
  cfi.FontWeight = FW_NORMAL;
  std::wcscpy(cfi.FaceName, L"Terminal");
  SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
#endif // DEBUG

  //Load window parallel thread
  sf::Thread thread(&loadScreen);
  thread.launch();

  //Create and init texture set

#ifdef DEBUG
  debug(L"Loading tileset");
#endif // DEBUG
  txset = new TextureSet;
  txset->right[0].loadFromFile("assets\\textures\\player\\right0.bmp");
  txset->right[1].loadFromFile("assets\\textures\\player\\right1.bmp");
  txset->down[0].loadFromFile("assets\\textures\\player\\down0.bmp");
  txset->down[1].loadFromFile("assets\\textures\\player\\down1.bmp");
  txset->down[2].loadFromFile("assets\\textures\\player\\down2.bmp");
  txset->down[3].loadFromFile("assets\\textures\\player\\down3.bmp");
  txset->left[0].loadFromFile("assets\\textures\\player\\left0.bmp");
  txset->left[1].loadFromFile("assets\\textures\\player\\left1.bmp");
  txset->up[0].loadFromFile("assets\\textures\\player\\up0.bmp");
  txset->up[1].loadFromFile("assets\\textures\\player\\up1.bmp");
  txset->up[2].loadFromFile("assets\\textures\\player\\up2.bmp");
  txset->up[3].loadFromFile("assets\\textures\\player\\up3.bmp");
  for(uint8_t i = 0; i < 15; i++) {
    txset->death[i].loadFromFile("assets\\textures\\player\\death" + std::to_string(i) + ".bmp");
  }

#ifdef DEBUG
  debug(L"Loading fonts");
#endif // DEBUG
  fsys = new sf::Font;
  fsys->loadFromFile("assets\\font\\fsys.ttf");

  //Delay
  sf::sleep(sf::seconds(1));

  //Setup player
  pl.spr.setOrigin(pl.spr.getGlobalBounds().width / 2, pl.spr.getGlobalBounds().height / 2);
  pl.spr.setScale(sf::Vector2f(5, 5));
  pl.spr.setTexture(txset->down[0]);

  //Create fullscreen window
  sf::RenderWindow window(sf::VideoMode(ScrW, ScrH), "shipgame", sf::Style::Fullscreen);
  window.setVerticalSyncEnabled(true);

  //Stopping load window
  load = false;
  thread.wait();

  selectmap(window);

  //Init map
  Map map;
  map.loadMap(mapsContainer[0]);

  //Event
  sf::Event event;

  //View
  sf::View view;
  view.setSize(ScrW, ScrH);
  view.setCenter(pl.spr.getPosition());

  //Rect whiting screen when player done
  sf::RectangleShape endRect(sf::Vector2f(ScrW, ScrH));
  endRect.setFillColor(sf::Color(255, 255, 255, 0));

  sf::Text youDied("You Died!", *fsys, 100), youWin("You Win!", *fsys, 100), pressKey("Press Alt+F4 to exit game...", *fsys, 16);
  youDied.setFillColor(sf::Color::Black);
  youDied.setPosition(sf::Vector2f(ScrW / 2, ScrH / 2 - 150));
  youDied.setOrigin(sf::Vector2f(youDied.getGlobalBounds().width / 2, 0));
  youWin.setFillColor(sf::Color::Black);
  youWin.setPosition(sf::Vector2f(ScrW / 2, ScrH / 2 - 150));
  youWin.setOrigin(sf::Vector2f(youWin.getGlobalBounds().width / 2, 0));
  pressKey.setFillColor(sf::Color::Black);
  pressKey.setPosition(sf::Vector2f(ScrW / 2, ScrH / 2 + 20));
  pressKey.setOrigin(sf::Vector2f(pressKey.getGlobalBounds().width / 2, 0));
  //Player speed
  uint16_t speed = 4;
  //
  uint16_t endAnimStat = 0;
  //Looped pressed security
  bool pressed = false;
  bool playerDieEnd = false;
  bool playerWinEnd = false;
  //Main loop
  while(window.isOpen()) {
    //Event loop
    while(window.pollEvent(event)) {
      if(event.type == sf::Event::Closed) {
        window.close();
      }
    }
    //Player control
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
      pl.move(speed, Direction::Up);
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
      pl.move(speed, Direction::Down);
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
      pl.move(speed, Direction::Left);
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
      pl.move(speed, Direction::Right);
    }
    //Fire
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
      if(!pressed) {
        pl.fire();
        pressed = true;
      }
    }
    else {
      pressed = false;
    }

    //View setting
    view.setCenter(pl.spr.getPosition());
    //Death animation
    if(playerDied) {
      if(endAnimStat > 254) {
        playerDieEnd = true;
      }
      else {
        endAnimStat++;
        endRect.setFillColor(sf::Color(255, 255, 255, endAnimStat));
      }
    }
    //Win animation
    if(playerWin) {
      if(endAnimStat > 253) {
        playerWinEnd = true;
      }
      else {
        endAnimStat += 2;
        endRect.setFillColor(sf::Color(255, 255, 255, endAnimStat));
      }
    }

    pl.update();
    map.update();

    //Draw
    if(playerDieEnd) {
      window.clear(sf::Color::White);
      window.setView(window.getDefaultView());
      window.draw(youDied);
      window.draw(pressKey);
    }
    else if(playerWinEnd) {
      window.clear(sf::Color::White);
      window.setView(window.getDefaultView());
      window.draw(youWin);
      window.draw(pressKey);
    }
    else {
      window.clear(sf::Color(50, 0, 50));
      window.setView(view);
      map.draw(window, pl.spr.getPosition());
      window.draw(pl);
      window.setView(window.getDefaultView());
      pl.drawUI(window);
      window.draw(endRect);
    }
    window.display();
  }
#ifdef DEBUG
  debug(L"Main loop exited");
#endif // DEBUG
}
