#include "main.hpp"

#include "Randomizer.hpp"
#include "Context.hpp"
#include "Level.hpp"
#include "Gui.hpp"
#include "Player.hpp"
#include "Magazine.hpp"
#include "Menu.hpp"
#include "Game.hpp"

int submain() {
  random = new Randomizer;
  context = new Context;
  level = new Level;
  gui = new Gui;
  player = new Player;
  magazine = new Magazine;

  context->create();

  Menu menu;

back:
  switch(menu.run()) {
    case 0:
      break;
    case 1:
    {
      Game().run();
      goto back;
    }
  }

  delete magazine;
  delete player;
  delete gui;
  delete level;
  delete context;
  delete random;

  return EXIT_SUCCESS;
}

#ifdef DEBUG
int main() {

  SetConsoleTitleW(L"WarGame Debug");
#else
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
#endif // DEBUG

  try {
    return submain();
  }
  catch(const Exeption& exeption) {
#ifdef DEBUG
    std::wcerr << L"Shit happened!" << std::endl << exeption.wtf();
#else
    MessageBoxW(NULL, exeption.wtf().c_str(), L"Shit happened!", MB_ICONERROR | MB_OK);
#endif // DEBUG
}
  return EXIT_FAILURE;
}
