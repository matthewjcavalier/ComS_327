#ifndef CHARACTER_H
  #define CHARACTER_H
  #include "Character.h"
#endif

Character::Character() {
}
void Character::takeTurn() {
  cout << "generic Character took turn" << endl;
}

PC::PC(Coordinate coord, int speed, Dungeon& dun, int nextEventTime) {
  symbol = '@';
  this->coord = coord;
  this->speed = speed;
  this->dun = dun;
  this->nextEventTime = nextEventTime;
}

void PC::takeTurn() {
  cout<<"PC took turn"<<endl;
}

NPC::NPC(Coordinate coord, int speed, Dungeon& dun, int nextEventTime, char type) {
  this->coord = coord;
  this->speed = speed;
  this->dun = dun;
  this->type = type;
  this->nextEventTime = nextEventTime;
}

void NPC::takeTurn() {
  cout<<"NPC took turn"<<endl;
}
