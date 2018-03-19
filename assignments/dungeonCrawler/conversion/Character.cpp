#define CHARACTER_H
#include "Character.h"


using namespace std;

/**
 * @brief Construct a new Character:: Character object
 * 
 */
Character::Character() {
  coord = {0,0};
  symbol = '&';
  speed = -1;
  nextEventTime = -1;
  dun = new Dungeon;
  id = -1;
}
/**
 * @brief Destroy the Character:: Character object
 * 
 */
Character::~Character() {}
/**
 * @brief make the generic character take their turn
 * 
 * @return int 
 */
int Character::takeTurn() {
  cout << "generic Character took turn" << endl;
  return 0;
}
/**
 * @brief make the character move in a random fashion
 *        but only allowing for movement into an open space
 * 
 * @return int the id of any character that was killed during the movement, 0 for no character found
 */
int Character::moveRand() {
  int newX = 0;
  int newY = 0;
  int modX = 0;
  int modY = 0;
  int foundId;

  while(dun->map[newY][newX].hardness != 0) {
    if(rand() % 2 == 0) {
      modX = (rand() % 2 == 0) ? 1 : -1;
    } else {
      modX = 0;
    }
    if(rand() % 2 == 0) {
      modY = (rand() % 2 == 0) ? 1 : -1;
    } else {
      modY = 0;
    }
    newX = coord.x + modX;
    newY = coord.y + modY;
  }
  
  dun->updateSpace(coord, NULL);
  foundId = getCharacterId(coord);
  coord = {newY, newX};
  dun->updateSpace(coord, this);
  return foundId;
}
/**
 * @brief  move the character in the direction of the
 *         coordinate passed in.
 * 
 * @param moveingTo 
 * @return int the id of the character killed during the movement, 0 for no character found
 */
int Character::moveToward(Coordinate moveingTo) {
  Coordinate to = coord;
  if(to.y != moveingTo.y) {
    to.y += (to.y > moveingTo.y) ? -1 : 1;
  }
  if(to.x != moveingTo.x) {
    to.x += (to.x > moveingTo.x) ? -1 : 1;
  }
  return moveTo(to);
}

/**
 * @brief  move the character to the input coordinate
 * 
 * @param to 
 * @return int the id of the character killed during the movement, 0 for no character found
 */
int Character::moveTo(Coordinate to) {
  int foundId = getCharacterId(to);
  if(dun->isOpenSpace(coord)) {
    dun->updateSpace(coord, NULL);
    coord = to;
    dun->updateSpace(coord, this);
  }
  return foundId;
}

/**
 * @brief Gets the id of the character in the dungeon at the given coordinate
 * 
 * @param loc 
 * @return int id of the character at the coord, 0 if none found
 */
int Character::getCharacterId(Coordinate loc) {
  if(dun->charMap[loc.y][loc.x] != NULL) {
    return dun->charMap[loc.y][loc.x]->id;
  }
  return 0;
}

/**
 * @brief Construct a new PC::PC object
 * 
 * @param id 
 * @param coord 
 * @param speed 
 * @param dun 
 * @param nextEventTime 
 */
PC::PC(int id, Coordinate coord, int speed, Dungeon* dun, int nextEventTime) {
  symbol = '@';
  this->coord = coord;
  this->speed = speed;
  this->dun = dun;
  this->nextEventTime = nextEventTime;
  this->id = id;
  dun->updateSpace(coord, this);
  setupDunMap();
  sightDist = 5;
}

/**
 * @brief Destroy the PC::PC object
 * 
 */
PC::~PC(){}

/**
 * @brief make the PC take their turn
 * 
 * @return int  the id of any charcter killed during the turn
 */
int PC::takeTurn() {
  nextEventTime += 1000/speed;
  int userPressed;
  movementResDTO res;
  updateDunMap();
  drawDunMap();
  clearMessageArea();
  do {
    userPressed = getch();
    if(DEBUG) {
      if(userPressed == 'f') {
        dun->draw();
      }
      if(userPressed == 't') {
        printMessage("Now in targeting mode");
        res.killed = startTeleportMode();
        tryToMove(coord);
        updateDunMap();
        clearMessageArea();
        drawDunMap();
        res.success = true;
      }
    }
    // move up left
    if(userPressed == '7' || userPressed == 'y') {
      res = tryToMove({coord.y - 1, coord.x -1});
    }
    // move up
    if(userPressed == '8' || userPressed == 'k') {
      res = tryToMove({coord.y - 1, coord.x});
    }
    // move up right
    if(userPressed == '9' || userPressed == 'u') {
      res = tryToMove({coord.y - 1, coord.x + 1});
    }
    // move right
    if(userPressed == '6' || userPressed == 'l') {
      res = tryToMove({coord.y, coord.x + 1});
    }
    // move down right
    if(userPressed == '3' || userPressed == 'n') {
      res = tryToMove({coord.y + 1, coord.x + 1});
    }
    // move down
    if(userPressed == '2' || userPressed == 'j') {
      res = tryToMove({coord.y + 1, coord.x});
    }
    // move down left
    if(userPressed == '1' || userPressed == 'b') {
      res = tryToMove({coord.y + 1, coord.x - 1});
    }
    // move left
    if(userPressed == '4' || userPressed == 'h') {
      res = tryToMove({coord.y, coord.x - 1});
    }
    // rest for a turn
    if(userPressed == '5' || userPressed == ' ') {
      res.success = true; 
    }
    // Quit
    if(userPressed == 'Q') {
      res.success = true; 
      res.killed = this->id;
    }
    // go down stairs
    if(userPressed == '>') {
      if(dun->map[coord.y][coord.x].type == DOWNSTAIR) {
        resetDunMap();
        return MOVE_BETWEEN_FLOORS;
      }
    }
    // go up stairs
    if(userPressed == '<') {
      if(dun->map[coord.y][coord.x].type == UPSTAIR) {
        resetDunMap();
        return MOVE_BETWEEN_FLOORS;
      }
    }
    // go up stairs
    if(userPressed == 'm') {
      showMonsterList();
    }
  } while(!res.success);
  dun->updateDistMaps();
  return res.killed;
}

/**
 * @brief Shows the monster list menue
 * 
 */
void PC::showMonsterList() {
  int topLeft_x = 13;
  int topLeft_y = 5;
  int vertBorderWidth = 2;
  int horBorderWidth = 5;
  int stringAreaWidth = 50;
  int currentTopMonst = 0;
  int numMonstersShown = 5;
  int userPressed;
  bool end = false;

  // init monster list
  vector<string> monsterList;
  
  // fill the list
  monsterList = fillMonsterList();

  do {
    drawMonsterBox(topLeft_y, topLeft_x, horBorderWidth, vertBorderWidth, numMonstersShown, stringAreaWidth);

    // populate the monster list
    for(int i = currentTopMonst; i < (int)monsterList.size() && i < currentTopMonst + numMonstersShown; i++) {
      drawString({topLeft_y + i - currentTopMonst + vertBorderWidth + 1, topLeft_x + horBorderWidth}, (char*)monsterList[i].c_str());
      fflush(stdout);
    }
    do {
      userPressed = getch();
      if(userPressed != ERR) {
        if(userPressed == KEY_DOWN) {
          drawString({0,0}, (char*)string("DOWN pressed").c_str());
          if((int)monsterList.size() > numMonstersShown) {
            currentTopMonst = (currentTopMonst < (int)monsterList.size() - numMonstersShown)? currentTopMonst + 1 : (int)monsterList.size() - numMonstersShown;
          }
        } else if(userPressed == KEY_UP) {
          drawString({0,0}, (char*)string("UP pressed").c_str());
          if((int)monsterList.size() > numMonstersShown) {
            currentTopMonst = (currentTopMonst > 0)? currentTopMonst - 1 : 0;
          }
        } else if(userPressed == 27) {
          if(getch() == ERR) {
            drawString({0,0}, (char*)string("ESC pressed").c_str());
            end = true;
          }
        }
      }
    } while(userPressed == ERR);
  } while (!end);
  drawDunMap();
}

/**
 * @brief Draws the box that the monster list will be shown in to screen
 * 
 * @param topLeft_y 
 * @param topLeft_x 
 * @param horBorderWidth 
 * @param vertBorderWidth 
 * @param numMonstersShown 
 * @param stringAreaWidth 
 */
void PC::drawMonsterBox(int topLeft_y, int topLeft_x, int horBorderWidth, int vertBorderWidth, int numMonstersShown, int stringAreaWidth) {
  for(int row = topLeft_y; row < topLeft_y + horBorderWidth + numMonstersShown - 1; row++) {
    for(int col = topLeft_x; col < topLeft_x + vertBorderWidth + stringAreaWidth - 1; col++) {
      if(row == topLeft_y || row == topLeft_y + horBorderWidth + numMonstersShown - 2) {
        drawCharacter({row + 1, col}, '_');
      } else if(col == topLeft_x || col == topLeft_x + vertBorderWidth + stringAreaWidth - 2) {
        drawCharacter({row + 1, col}, '|');
      } else {
        drawCharacter({row + 1, col}, ' ');
      }
    }
  }
}

/**
 * @brief fills a vector with strings that will be shown in the monster list menue
 * 
 * @return vector<string> 
 */
vector<string> PC::fillMonsterList() {
  vector<string> monsterList;
  for(auto row : dun->charMap) {
    for(Character* character : row) {
      if(character != NULL && character->symbol != '@') {
        monsterList.push_back(genMonsterString(this->coord.y - character->coord.y, 
              this->coord.x - character->coord.x, character->symbol));
      }
    }
  }
  return monsterList;
}

/**
 * @brief  generates a string based upon the attributes passed in as parameters for where
 *         a monster is in relation to the PC
 * 
 * @param ydiff 
 * @param xdiff 
 * @param symbol 
 * @return string 
 */
string PC::genMonsterString(int ydiff, int xdiff, char symbol) {
  ostringstream ret;
  if(ydiff == 0) {
    ret << symbol << " is " << abs(xdiff) << " to the " << ((xdiff > 0) ? "West" : "East");
  } else if(xdiff == 0) {
    ret << symbol << " is " << abs(ydiff) << " to the " << ((ydiff > 0) ? "North" : "South");
  } else {
    ret << symbol << " is " << abs(ydiff) << " to the " << ((ydiff > 0) ? "North" : "South") << " and " << abs(xdiff) << " to the " << ((xdiff > 0) ? "West" : "East");
  }
  return ret.str();
}

/**
 * @brief Sets up the PC's personal dungeon map of what has been seen geographically
 * 
 */
void PC::setupDunMap() {
  TileType spot = ROCK;
  dunMap.resize(MAX_HEIGHT);
  for(int row = 0; row < MAX_HEIGHT; row++) {
    dunMap[row].resize(MAX_WIDTH, spot);
  }
}

/**
 * @brief tries to move the character to the input coordinate
 * 
 * @param to 
 * @return movementResDTO a DTO that contains if the movement was successfull and the id of a character that was killed if any
 */
movementResDTO PC::tryToMove(Coordinate to) {
  movementResDTO res;
  if(dun->isOpenSpace(to)) {
    dun->updateSpace(coord, NULL);
    res.killed = getCharacterId(to);
    coord = to;
    dun->updateSpace(coord, this);
    res.success = true;
    // update the personal dungeon map
    updateDunMap();
  }
  return res;
}

/**
 * @brief updates the PC's personal dungeon map
 * 
 */
void PC::updateDunMap() {
  for(int row = coord.y - sightDist; row < coord.y + sightDist; row++) {
    for(int col = coord.x - sightDist; col < coord.x + sightDist; col++) {
      if(row < MAX_HEIGHT && col < MAX_WIDTH &&
         dun->canSeeFrom(coord, {row,col})) {

        dunMap[row][col] = dun->map[row][col].type;
      }
    }
  }
}

/**
 * @brief Draws the PC's dungeon map taking into account how far the PC can see
 * 
 */
void PC::drawDunMap() {
  bool canSee = false;
  // draw known map
  attron(A_STANDOUT);
  for(int row = 0; row < MAX_HEIGHT; row++) {
    for(int col = 0; col < MAX_WIDTH; col++) {
      if(row < MAX_HEIGHT && col < MAX_WIDTH) {
        drawCharacter({row+1,col}, getTileSym(dunMap[row][col]));
      }
    }
  }
  attroff(A_STANDOUT);
  attron(A_DIM);
  // draw monsters in sight radius
  for(int row = coord.y - sightDist; row < coord.y + sightDist; row++) {
    for(int col = coord.x - sightDist; col < coord.x + sightDist; col++) {
      if(row > 0 && row < MAX_HEIGHT &&
         col > 0 && col < MAX_WIDTH) {
          canSee = dun->canSeeFrom(coord, {row,col});
          if(canSee) {
            if(dun->charMap[row][col] == NULL) {
              if(dunMap[row][col] == UPSTAIR || dunMap[row][col] == DOWNSTAIR) {
                attron(A_BOLD);
              }
              drawCharacter({row+1,col}, getTileSym(dunMap[row][col]));
              attroff(A_BOLD);
            } else {
              attron(A_BOLD);
              drawCharacter({row+1,col}, dun->charMap[row][col]->symbol);
              attroff(A_BOLD);
            }
          }
      }
    }
  }
  attroff(A_DIM);
}

/**
 * @brief Set all indicies in the PC dungeon map to Rock
 * 
 */
void PC::resetDunMap() {
  for(int row = 0; row < MAX_HEIGHT; row++) {
    for(int col = 0; col < MAX_WIDTH; col++) {
      dunMap[row][col] = ROCK;
    }
  }
}

/**
 * @brief Starts up the the teleportaion mode for the PC
 * 
 * @return int  returns the id of any monster that the PC kills when it teleports
 */
int PC::startTeleportMode() {
  int userPressed;
  Coordinate teleportTo = coord;
  drawCharacter({teleportTo.y + 1, teleportTo.x}, TELEPORT_CHAR);
  bool done = false;
  dun->draw();
  do {
    userPressed = getch();
    // move up left
    if(userPressed == '7' || userPressed == 'y') {
      if(isInDun({teleportTo.y - 1, teleportTo.x -1})){
        drawNormalSpot(teleportTo);
        teleportTo.y--;
        teleportTo.x--;
        drawCharacter({teleportTo.y + 1, teleportTo.x}, TELEPORT_CHAR);
      }
    }
    // move up
    if(userPressed == '8' || userPressed == 'k') {
      if(isInDun({teleportTo.y - 1, teleportTo.x})){
        drawNormalSpot(teleportTo);
        teleportTo.y--;
        drawCharacter({teleportTo.y + 1, teleportTo.x}, TELEPORT_CHAR);
      }
    }
    // move up right
    if(userPressed == '9' || userPressed == 'u') {
      if(isInDun({teleportTo.y - 1, teleportTo.x +1})){
        drawNormalSpot(teleportTo);
        teleportTo.y--;
        teleportTo.x++;
        drawCharacter({teleportTo.y + 1, teleportTo.x}, TELEPORT_CHAR);
      }
    }
    // move right
    if(userPressed == '6' || userPressed == 'l') {
      if(isInDun({teleportTo.y, teleportTo.x +1})){
        drawNormalSpot(teleportTo);
        teleportTo.x++;
        drawCharacter({teleportTo.y + 1, teleportTo.x}, TELEPORT_CHAR);
      }
    }
    // move down right
    if(userPressed == '3' || userPressed == 'n') {
      if(isInDun({teleportTo.y + 1, teleportTo.x +1})){
        drawNormalSpot(teleportTo);
        teleportTo.y++;
        teleportTo.x++;
        drawCharacter({teleportTo.y + 1, teleportTo.x}, TELEPORT_CHAR);
      }
    }
    // move down
    if(userPressed == '2' || userPressed == 'j') {
      if(isInDun({teleportTo.y + 1, teleportTo.x})){
        drawNormalSpot(teleportTo);
        teleportTo.y++;
        drawCharacter({teleportTo.y + 1, teleportTo.x}, TELEPORT_CHAR);
      }
    }
    // move down left
    if(userPressed == '1' || userPressed == 'b') {
      if(isInDun({teleportTo.y + 1, teleportTo.x -1})){
        drawNormalSpot(teleportTo);
        teleportTo.y++;
        teleportTo.x--;
        drawCharacter({teleportTo.y + 1, teleportTo.x}, TELEPORT_CHAR);
      }
    }
    // move left
    if(userPressed == '4' || userPressed == 'h') {
      if(isInDun({teleportTo.y, teleportTo.x -1})){
        drawNormalSpot(teleportTo);
        teleportTo.x--;
        drawCharacter({teleportTo.y + 1, teleportTo.x}, TELEPORT_CHAR);
      }
    }
    if(userPressed == 'r') {
      done = true;
      teleportTo.y = rand() % (MAX_HEIGHT - 1) + 1;
      teleportTo.x = rand() % (MAX_WIDTH - 1) + 1;
    }
    if(userPressed == 't') {
      done = true;
    }
    if(userPressed == 'Q') {
      teleportTo = coord;
      done = true;
    }
  } while(!done);
  int foundId = teleport(teleportTo);
  drawNormalSpot(teleportTo);
  return foundId;
}

/**
 * @brief Draw the character at the given spot
 * 
 * @param spot 
 */
void PC::drawNormalSpot(Coordinate spot) {
  if(dun->charMap[spot.y][spot.x] != NULL) {
    drawCharacter({spot.y + 1, spot.x}, dun->charMap[spot.y][spot.x]->symbol);
  } else {
    drawCharacter({spot.y + 1, spot.x}, getTileSym(dun->map[spot.y][spot.x].type));
  }
}

/**
 * @brief teleport the PC to the input coordinate
 * 
 * @param to 
 * @return int the id of any character that was killed when the PC teleported, 0 if none killed
 */
int PC::teleport(Coordinate to) {
  int foundId = getCharacterId(to);
  dun->updateSpace(coord, NULL);
  coord = to;
  dun->updateSpace(coord, this);
  return foundId;
}
/**
 * @brief Construct a new NPC::NPC object
 * 
 * @param id 
 * @param coord 
 * @param speed 
 * @param dun 
 * @param nextEventTime 
 * @param type 
 * @param pc 
 */
NPC::NPC(int id, Coordinate coord, int speed, Dungeon* dun, int nextEventTime, char type, PC* pc) {
  this->coord = coord;
  this->speed = speed;
  this->dun = dun;
  this->type = type;
  this->nextEventTime = nextEventTime;
  this->symbol = getSymbol(type);
  this->pc = pc;
  this->id = id;
  lastSeenPCLoc = {0,0};
  dun->updateSpace(coord, this);
  setTurnLogic();
}

/**
 * @brief Destroy the NPC::NPC object
 * 
 */
NPC::~NPC(){}

/**
 * @brief sets which movement routine the NPC will use when it takes it's turn based on what type of Monster it is
 * 
 */
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
    case 0b1001:
      turnLogic = &NPC::movement1001;
      break;
    case 0b1010:
      turnLogic = &NPC::movement1010;
      break;
    case 0b1011:
      turnLogic = &NPC::movement1011;
      break;
    case 0b1100:
      turnLogic = &NPC::movement1100;
      break;
    case 0b1101:
      turnLogic = &NPC::movement1101;
      break;
    case 0b1110:
      turnLogic = &NPC::movement1110;
      break;
    case 0b1111:
      turnLogic = &NPC::movement1111;
      break;
    default:
      turnLogic = &Character::moveRand;
  }
}

/**
 * @brief Have the NPC take it's turn
 * 
 * @return int 
 */
int NPC::takeTurn() {
  nextEventTime += 1000/speed;
  int foundId = (this->*turnLogic)();
  return foundId;
}

/**
 * @brief updtaes the last known location of the PC. If the PC is in sight, the location
 *        saved is accurate. If the PC hasn't been seen the location is 0,0. If the monster
 *        reaches the last known PC location and the pc isn't killed, the location is set to 0,0
 * 
 */
void NPC::updatePCLoc() {
  if(dun->canSeeFrom(coord, pc->coord)) {
    lastSeenPCLoc = pc->coord;
  } else if(coord.x == lastSeenPCLoc.x && coord.y == lastSeenPCLoc.y) {
    lastSeenPCLoc = {0,0};
  }
}

/**
 * @brief movement routine where the Monster is Intelligent
 * 
 * @return int 
 */
int NPC::movement0001() {
  updatePCLoc();
  if(lastSeenPCLoc.x != 0 && lastSeenPCLoc.y != 0) {
    return moveToward(lastSeenPCLoc);
  } else {
    return moveRand();
  }
}
/**
 * @brief movement routine where the Monster is telepathic
 * 
 * @return int 
 */
int NPC::movement0010(){
  return moveToward(pc->coord);
}
/**
 * @brief movement routine where the Monster is intelligent and telepathic
 * 
 * @return int 
 */
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
/**
 * @brief movement routine where the Monster is able to tunnel
 * 
 * @return int 
 */
int NPC::movement0100(){
  return moveRandTunneling();
}

/**
 * @brief movement routine where the Monster is able to tunnel and is intelligent
 * 
 * @return int 
 */
int NPC::movement0101(){
  updatePCLoc();
  if(lastSeenPCLoc.x != 0 && lastSeenPCLoc.y != 0) {
    return moveToward(lastSeenPCLoc);
  } else {
    return moveRandTunneling();
  }
}

/**
 * @brief movement routine where the Monster is able to tunnel and is telepathic
 * 
 * @return int 
 */
int NPC::movement0110(){
  return moveTowardTunneling(pc->coord);
}
/**
 * @brief movement routine where the Monster is able to tunnel, is intelligent, and is telepathic
 * 
 * @return int 
 */
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
  }
  return foundID;
}

/**
 * @brief movement routine where the Monster is erratic and intelligent
 * 
 * @return int 
 */
int NPC::movement1001() {
  return (rand() % 2) ? moveRand() : movement0001();
}
/**
 * @brief Amovement routine where the Monster is erratic and telepathic
 * 
 * @return int 
 */
int NPC::movement1010(){
  return (rand() % 2) ? moveRand() : movement0010();
}

// Erratic Tunneling Telepathy Intelligence
/**
 * @brief movement routine where the Monster is erratic and is intelligent and telepathic
 * 
 * @return int 
 */
int NPC::movement1011(){
  return (rand() % 2) ? moveRand() : movement0011();
}

/**
 * @brief movement routine where the Monster is erratic and can tunnel
 * 
 * @return int 
 */
int NPC::movement1100(){
  return movement0100();
}

/**
 * @brief  movement routine where the Monster is erratic and is both able to tunnel and is intelligent
 * 
 * @return int 
 */
int NPC::movement1101(){
  return (rand() % 2) ? moveRand() : movement0101();
}

/**
 * @brief movement routine where the Monster is erratic and telepathic and able to tunnel
 * 
 * @return int 
 */
int NPC::movement1110(){
  return (rand() % 2) ? moveRand() : movement0110();
}

/**
 * @brief movement routine where the Monster is erratic, able to tunnel, telepathic, and is intelligent 
 * 
 * @return int 
 */
int NPC::movement1111(){
  return (rand() % 2) ? moveRand() : movement0111();
}

/**
 * @brief move the monster toward the input coordinate while allowing for tunneling
 * 
 * @param coord 
 * @return int 
 */
int NPC::moveTowardTunneling(Coordinate coord) {
  Coordinate to = this->coord;
  int foundId;
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
    foundId = getCharacterId(to);
    this->coord = {to.y, to.x};
    dun->updateSpace(this->coord, this);
  }
  return foundId;
}

/**
 * @brief Move the monster in a random direction allowing them to tunnel
 * 
 * @return int 
 */
int NPC::moveRandTunneling() {
  int newX = 0;
  int newY = 0;
  int modX = 0;
  int modY = 0;
  int foundId = 0;

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
    foundId = getCharacterId(coord);
    coord = {newY, newX};
    dun->updateSpace(coord, this);
  }
  return foundId;
}

/**
 * @brief Get the symbol for a monster type
 * 
 * @param type 
 * @return char 
 */
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

/**
 * @brief Returns if the coordinate is in the dungeon and is inside the border
 * 
 * @param coord 
 * @return true 
 * @return false 
 */
bool isInDun(Coordinate coord) {
  return coord.y > 0 && coord.y < MAX_HEIGHT - 1 && coord.x > 0 && coord.x < MAX_WIDTH - 1;
}
