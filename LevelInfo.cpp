#include "LevelInfo.hpp"

#include <fstream>
#include <WinSock2.h>
#include "Tile.hpp"

std::wstring readString(std::ifstream& file) {
  std::string str;
  char ch = 0;
  while(true) {
    ch = file.get();
    if(ch == 0 || ch == -1) break;
    str += ch;
  }
  return wide(str);
}

bool LevelInfo::load(const std::string filename) {
  std::ifstream file(filename, std::ios::binary);
  
  if(!file) {
    return false;
  }

  char header[6]{};
  if(file.read(header, 6).gcount() != 6) {
    return false;
  }

  if(strncmp(header, "fLwGsM", 6) == 0) {
    pack = false;

    name = readString(file);
    author = readString(file);
  }
  else if(strncmp(header, "fLwGmE", 6) == 0) {
    pack = true;

    name = readString(file);
    author = readString(file);

    TileInt size = 0;
    if(file.read(reinterpret_cast<char*>(&size), sizeof(size)).gcount() != sizeof(size)) {
      return false;
    }
    size = ntohs(size);
    file.close();
  }
  else {
    return false;
  }

  filename_ = filename;
  return true;
}
