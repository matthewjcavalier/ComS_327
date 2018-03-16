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
  //int id = moveRand();
  dun->updateDistMaps();
  return 0;
//  return id;
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
    case 0b0010:
      turnLogic = &NPC::movement0010;
      break;
    case 0b0011:
      turnLogic = &NPC::movement0011;
      break;
    case 0b0100:
      turnLogic = &NPC::movement0100;
      break;
    case 0b0101:
      turnLogic = &NPC::movement0101;
      break;
    case 0b0110:
      turnLogic = &NPC::movement0110;
      break;
    case 0b0111:
      turnLogic = &NPC::movement0111;
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

int NPC::movement0001() {
  updatePCLoc();
  if(lastSeenPCLoc.x != 0 && lastSeenPCLoc.y != 0) {
    return moveToward(lastSeenPCLoc);
  } else {
    return moveRand();
  }
}
int NPC::movement0010(){
  return moveToward(pc->coord);
}
int NPC::movement0011(){
  Coordinate next = coord;
  int min = INT_MAX;
  for(int y = coord.y - 1; y < coord.y + 2; y++) {
    for(int x = coord.x - 1; x < coord.x + 2; x++) {
      if(dun->openMap[y][x] < min) {
        min = dun->openMap[y][x];
        next = {y,x};
      }
    }
  }
  return moveTo(next);
}
int NPC::movement0100(){
  return moveRandTunneling();
}
int NPC::movement0101(){
  updatePCLoc();
  if(lastSeenPCLoc.x != 0 && lastSeenPCLoc.y != 0) {
    return moveToward(lastSeenPCLoc);
  } else {
    return moveRandTunneling();
  }
}

// Erratic Tunneling Telepathy Intelligence
int NPC::movement0110(){
  return moveTowardTunneling(pc->coord);
}
int NPC::movement0111(){
  Coordinate next = coord;
  int min = INT_MAX;
  int foundID = 0;
  for(int y = coord.y - 1; y < coord.y + 2; y++) {
    for(int x = coord.x - 1; x < coord.x + 2; x++) {
      if(dun->tunnelMap[y][x] < min) {
        min = dun->tunnelMap[y][x];
        next = {y,x};
      }
    }
  }

  if(dun->map[next.y][next.x].hardness > 0) {
    if(dun->map[next.y][next.x].hardness < 85) {
      dun->map[next.y][next.x].hardness = 0;
    } else {
      dun->map[next.y][next.x].hardness -= 85;
    }
    dun->updateDistMaps();
  }

  if(dun->map[next.y][next.x].hardness <= 0) {
    if(dun->map[next.y][next.x].type != HALL && dun->map[next.y][next.x].type != ROOM) {
      dun->map[next.y][next.x].setType(0);
    }
    dun->updateSpace(this->coord, NULL);
    foundID = getCharacterId(next);
    this->coord = {next.y, next.x};
    dun->updateSpace(this->coord, this);
    dun->draw();
  }
  return foundID;
}

int NPC::moveTowardTunneling(Coordinate coord) {
  Coordinate to = this->coord;
  if(to.y != coord.y) {
    to.y += (to.y > coord.y) ? -1 : 1;
  }
  if(to.x != coord.x) {
    to.x += (to.x > coord.x) ? -1 : 1;
  }

  if(dun->map[to.y][to.x].hardness > 0) {
    if(dun->map[to.y][to.x].hardness < 85) {
      dun->map[to.y][to.x].hardness = 0;
    } else {
      dun->map[to.y][to.x].hardness -= 85;
    }
  }

  if(dun->map[to.y][to.x].hardness <= 0) {
    if(dun->map[to.y][to.x].type != HALL && dun->map[to.y][to.x].type != ROOM) {
      dun->map[to.y][to.x].setType(0);
    }
    dun->updateSpace(this->coord, NULL);
    id = getCharacterId(to);
    this->coord = {to.y, to.x};
    dun->updateSpace(this->coord, this);
    dun->draw();
  }
  return id;
}

int NPC::moveRandTunneling() {
  int newX = 0;
  int newY = 0;
  int modX = 0;
  int modY = 0;
  int id = 0;

  do {
    if(rand() % 2 == 0) {
      modX = (rand() % 2 == 0) ? 1 : -1;
    }
    if(rand() % 2 == 0) {
      modY = (rand() % 2 == 0) ? 1 : -1;
    }
    newX = coord.x + modX;
    newY = coord.y + modY;
  } while(dun->map[newY][newX].type == BORDER);
  
  if(dun->map[newY][newX].hardness > 0) {
    if(dun->map[newY][newX].hardness < 85) {
      dun->map[newY][newX].hardness = 0;
    } else {
      dun->map[newY][newX].hardness -= 85;
    }
  }

  if(dun->map[newY][newX].hardness <= 0) {
    if(dun->map[newY][newX].type != HALL && dun->map[newY][newX].type != ROOM) {
      dun->map[newY][newX].setType(0);
    }
    dun->updateSpace(coord, NULL);
    id = getCharacterId(coord);
    coord = {newY, newX};
    dun->updateSpace(coord, this);
    dun->draw();
  }
  return id;
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
