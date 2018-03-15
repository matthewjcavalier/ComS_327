#define CHARACTER_H
#include "Character.h"


using namespace std;

Character::Character() {
  coord = {0,0};
  symbol = '&';
  speed = -1;
  nextEventTime = -1;
  dun = new Dungeon;
  id = -1;
}
int Character::takeTurn() {
  cout << "generic Character took turn" << endl;
  return 0;
}

int Character::moveRand() {
  int newX = 0;
  int newY = 0;
  int modX = 0;
  int modY = 0;
  int id;

  while(dun->map[newY][newX].hardness != 0) {
    modX = (rand() % 2 == 0) ? 1 : -1;
    modY = (rand() % 2 == 0) ? 1 : -1;
    newX = coord.x + modX;
    newY = coord.y + modY;
  }
  
  dun->updateSpace(coord, NULL);
  id = getCharacterId(coord);
  coord = {newY, newX};
  dun->updateSpace(coord, this);
  dun->draw();
  return id;
}

int Character::moveToward(Coordinate coord) {
  Coordinate to = this->coord;
  if(to.y != coord.y) {
    to.y += (to.y > coord.y) ? -1 : 1;
  }
  if(to.x != coord.x) {
    to.x += (to.x > coord.x) ? -1 : 1;
  }
  return moveTo(to);
}

int Character::moveTo(Coordinate coord) {
  int id = getCharacterId(coord);
  if(dun->isOpenSpace(coord)) {
    dun->updateSpace(this->coord, NULL);
    this->coord = coord;
    dun->updateSpace(this->coord, this);
  }
  return id;
}

int Character::getCharacterId(Coordinate loc) {
  if(dun->charMap[loc.y][loc.x] != NULL) {
    return dun->charMap[loc.y][loc.x]->id;
  }
  return 0;
}

PC::PC(int id, Coordinate coord, int speed, Dungeon* dun, int nextEventTime) {
  symbol = '@';
  this->coord = coord;
  this->speed = speed;
  this->dun = dun;
  this->nextEventTime = nextEventTime;
  dun->updateSpace(coord, this);
}

int PC::takeTurn() {
  nextEventTime += 1000/speed;
  int id = moveRand();
  dun->updateDistMaps();
  return id;
}

NPC::NPC(int id, Coordinate coord, int speed, Dungeon* dun, int nextEventTime, char type, PC* pc) {
  this->coord = coord;
  this->speed = speed;
  this->dun = dun;
  this->type = type;
  this->nextEventTime = nextEventTime;
  this->symbol = getSymbol(type);
  this->pc = pc;
  lastSeenPCLoc = {0,0};
  dun->updateSpace(coord, this);
  setTurnLogic();
}

void NPC::setTurnLogic() {
  switch(type) {
    case 0b0001:
      turnLogic = &NPC::movement0001;
      break;
    default:
//      return [this](void) mutable{NPC::moveRand();};
      turnLogic = &Character::moveRand;
  }
}

int NPC::takeTurn() {
  nextEventTime += 1000/speed;
  int id = (this->*turnLogic)();
  dun->draw();
  return id;
}

void NPC::updatePCLoc() {
  if(dun->canSeeFrom(coord, pc->coord)) {
    lastSeenPCLoc = pc->coord;
  } else if(coord.x == lastSeenPCLoc.x && coord.y == lastSeenPCLoc.y) {
    lastSeenPCLoc = {0,0};
  }
}

// Erratic Tunneling Telepathy Intelligence
int NPC::movement0001() {
  updatePCLoc();
  if(lastSeenPCLoc.x != 0 && lastSeenPCLoc.y != 0) {
    return moveToward(lastSeenPCLoc);
  } else {
    return moveRand();
  }
}
int NPC::movement0010(){
  return 0;
}
int NPC::movement0011(){
  return 0;
}
int NPC::movement0100(){
  return 0;
}
int NPC::movement0101(){
  return 0;
}
int NPC::movement0110(){
  return 0;
}
int NPC::movement0111(){
  return 0;
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
