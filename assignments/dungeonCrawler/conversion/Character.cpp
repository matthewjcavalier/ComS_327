#define CHARACTER_H
#include "Character.h"


using namespace std;

Character::Character() {
  coord = {0,0};
  symbol = '&';
  speed = -1;
  nextEventTime = -1;
  dun = new Dungeon;
}
void Character::takeTurn() {
  cout << "generic Character took turn" << endl;
}

void Character::moveRand() {
  int newX = 0;
  int newY = 0;
  int modX = 0;
  int modY = 0;

  while(dun->map[newY][newX].hardness != 0) {
    modX = (rand() % 2 == 0) ? 1 : -1;
    modY = (rand() % 2 == 0) ? 1 : -1;
    newX = coord.x + modX;
    newY = coord.y + modY;
  }

  dun->updateSpace(coord, NULL);
  coord = {newY, newX};
  dun->updateSpace(coord, this);
}

PC::PC(Coordinate coord, int speed, Dungeon* dun, int nextEventTime) {
  symbol = '@';
  this->coord = coord;
  this->speed = speed;
  this->dun = dun;
  this->nextEventTime = nextEventTime;
  dun->updateSpace(coord, this);
}

void PC::takeTurn() {
  nextEventTime += 1000/speed;
  moveRand();
  dun->updateDistMaps();
  dun->draw();
}

NPC::NPC(Coordinate coord, int speed, Dungeon* dun, int nextEventTime, char type) {
  this->coord = coord;
  this->speed = speed;
  this->dun = dun;
  this->type = type;
  this->nextEventTime = nextEventTime;
  this->symbol = getSymbol(type);
  dun->updateSpace(coord, this);
  turnLogic = getTurnLogic();
}

void* NPC::getTurnLogic() {
  switch(type) {
    default:
      return &Character::moveRand;
  }
}

void NPC::takeTurn() {
  nextEventTime += 1000/speed;
  turnLogic();
}

char getSymbol(char type) {
  switch(type) {
    case 1:
      return '1';
    case 2:
      return '2';
    case 3:
      return '3';
    case 4:
      return '4';
    case 5:
      return '5';
    case 6:
      return '6';
    case 7:
      return '7';
    case 8:
      return '8';
    case 9:
      return '9';
    case 10:
      return 'A';
    case 11:
      return 'B';
    case 12:
      return 'C';
    case 13:
      return 'D';
    case 14:
      return 'E';
    case 15:
      return 'F';
    default:
      return '0';
  }
}
