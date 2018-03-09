#include "Character.h"

Character::Character() {
  this->x = 0;
  this->y = 0;
  this->symbol = 'M';
  this->speed = 10;
  this->nextEventTime = nextEventTime;
}
Character::Character(Dungeon& dun, int x, int y, char symbol, int speed, int nextEventTime) {
  this->dun = dun;
  this->x = x;
  this->y = y;
  this->symbol = symbol;
  this->speed = speed;
  this->nextEventTime = nextEventTime;
}
void Character::takeTurn() {
}

