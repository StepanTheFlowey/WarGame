#include <windows.h>
enum class ConsoleColor {
  Black         = 0,
  Blue          = 1,
  Green         = 2,
  Cyan          = 3,
  Red           = 4,
  Magenta       = 5,
  Brown         = 6,
  LightGray     = 7,
  DarkGray      = 8,
  LightBlue     = 9,
  LightGreen    = 10,
  LightCyan     = 11,
  LightRed      = 12,
  LightMagenta  = 13,
  Yellow        = 14,
  White         = 15
};

HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

void SetColor(int text, int background) {
  SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
}

void gotoxy (short x, short y) {
  COORD coordinates = { x, y };
  SetConsoleCursorPosition (hStdOut, coordinates);
}