#include "main.hpp"

#include "Level.hpp"
#include "Context.hpp"
#include "Player.hpp"
#include "Gui.hpp"
#include "Menu.hpp"
#include "Game.hpp"

int submain() {
  context = new Context;
  level = new Level;
  gui = new Gui;
  magazine = new Magazine;
  player = new Player;

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

  return EXIT_SUCCESS;
}

#ifdef DEBUG
int main() {
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