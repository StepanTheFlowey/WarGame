#include <Windows.h>
#include <iostream>
#include <fstream>
#include <conio.h>
using namespace std;
#include "consolefunctions.h"

char map[64][64];
unsigned char cursorX = 0,
cursorY = 0;
int main() {
  system("title Map editor");
  system("mode con lines=64 cols=64");
  SetColor(Black, White);
  for(short i1 = 0; i1 < 64; i1++) {
    for(short i2 = 0; i2 < 64; i2++) {
      cout << " ";
    }
  }
  while(true) {
    gotoxy(cursorX, cursorY);
    int key = getch();
    switch(key) {
      case 72:
      {
        cursorY--;
        if(cursorY > 64) {
          cursorY = 63;
        }
        break;
      }
      case 80:
      {
        cursorY++;
        if(cursorY > 64) {
          cursorY = 0;
        }
        break;
      }
      case 77:
      {
        cursorX++;
        if(cursorX > 64) {
          cursorX = 0;
        }
        break;
      }
      case 75:
      {
        cursorX--;
        if(cursorX > 64) {
          cursorX = 63;
        }
        break;
      }
      case 50:
      {
        map[cursorX][cursorY] = 2;
        SetColor(White, Red);
        cout << " ";
        break;
      }
      case 49:
      {
        map[cursorX][cursorY] = 1;
        SetColor(White, Black);
        cout << " ";
        break;
      }
      case 48:
      {
        map[cursorX][cursorY] = 0;
        SetColor(Black, White);
        cout << " ";
        break;
      }
      case 's':
      {
        ofstream file("map.txt");
        file << "char mapp[][65][65]=\n{";
        for(short i1 = 0; i1 < 64; i1++) {
          file << "{\n{";
          for(short i2 = 0; i2 < 64; i2++) {
            file << static_cast<short>(map[i2][i1]) << ",";
          }
          file << "0},\n";
        }
        file << "{}\n}\n}";
        break;
      }
    }
  }
}
