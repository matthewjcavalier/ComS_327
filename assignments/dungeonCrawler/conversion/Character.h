#ifndef IOSTREAM
  #define IOSTREAM
  #include <iostream>
#endif
#ifndef DUNGEON_H
  #define DUNGEON_H
  #include "dungeon.h"
#endif
#ifndef FUNCTIONAL
  #define FUNCTIONAL
  #include <functional>
#endif
#ifndef UNISTD_H
  #define UNISTD_H
  #include <unistd.h>
#endif
#ifndef STRING
  #define STRING
  #include <string>
#endif
#ifndef SSTREAM
  #define SSTREAM
  #include <sstream>
#endif
#ifndef DESC_H
  #define DESC_H
  #include "desc.h"
#endif
#ifndef OBJECT_H
  #define OBJECT_H
  #include "object.h"
#endif
#ifndef TYPEINFO
  #define TYPEINFO 
  #include <typeinfo>
#endif

#define PC_INVENTORY_SIZE 10
#define PC_EQUIP_SLOTS 12

using namespace std;

class Dungeon;

enum weaponSlot {
  WEAPON,
  OFFHAND,
  RANGED,
  ARMOR,
  HELMET,
  CLOAK,
  GLOVES,
  BOOTS,
  AMULET,
  LIGHT,
  RING1,
  RING2,
};

class movementResDTO {
  public:
    bool success;
    int killed;
    movementResDTO() {
      success = false;
      killed = 0;
    }
};

class Character {
  public:
    string name;
    string description;
    vector<string> colors;
    int hp;
    rollUp ad;
    int adBuff;

    Coordinate coord;
    char symbol;
    int speed;
    int nextEventTime;
    Dungeon* dun;
    int id;
    bool isBossMonster;

    Character();
    virtual ~Character();
    virtual int takeTurn();
    
    int getCharacterId(Coordinate loc);
    int attack(rollUp attackerAD, int attackerADbuff, Character* defender);
};

class PC : public Character {
  public:
    int baseSpeed;
    bool hasKilledBoss;
    object* inventory[PC_INVENTORY_SIZE];
    object* equipped[PC_EQUIP_SLOTS];
    vector<vector<TileType>> dunMap;
    PC(int id, Coordinate coord, int speed, Dungeon* dun, int nextEventTime);
    ~PC();
    int takeTurn();
  private:
    int sightDist;
    movementResDTO tryToMove(Coordinate to);
    void showMonsterList();
    vector<string> fillMonsterList();
    string genMonsterString(int ydiff, int xdiff, char symbol);
    void drawMonsterBox(int topLeft_y, int topLeft_x, int horBorderWidth, int vertBorderWidth, int numMonstersShown, int stringAreaWidth);
    void setupDunMap();
    void updateDunMap();
    void drawDunMap();
    void resetDunMap();
    int startTeleportMode();
    void drawNormalSpot(Coordinate spot);
    int teleport(Coordinate to);
    void pickUpItem();
    bool isInventoryFull();
    void addToInventory(object* objPtr);
    void updateCurrentStats();
    int putOnButtKickingOutfit();
    int getMoreNaked();
    int dropItLikeItsHot();
    int destroyItem();
    int checkPockets();
    int waitWhatAmIWearing();
    int inspectItem();
    int lookForSexyMonsters();
    int tryToEquipItem(int index);
    object* equipItem(object* obj);
    void tryToDropItem(int index);
    void placeOnFloor(object* obj);
    int describeEquipSlots();
    int tryToTakeOff(int index);
    int deleteItem(int index);
    int showItemDescription(int index);
    bool isWithinSight(Coordinate coord);
    void printMonsterInfo(Coordinate loc);
};

class NPC : public Character {
  public:
    char type;
    Coordinate lastSeenPCLoc;
    rollUp ad;
    PC* pc;
    NPC();
    NPC(int id, Coordinate coord, int speed, Dungeon* dun, int nextEventTime, char type, PC* pc);
    ~NPC();
    int takeTurn();
    void setTurnLogic();
    int moveRand();
    int moveTo(Coordinate coord);
    int moveToward(Coordinate coord);
  private:
    int (NPC::*turnLogic)();
    void updatePCLoc();
    int movement0001();
    int movement0010();
    int movement0011();
    int movement0100();
    int movement0101();
    int movement0110();
    int movement0111();
    int movement1000();
    int movement1001();
    int movement1010();
    int movement1011();
    int movement1100();
    int movement1101();
    int movement1110();
    int movement1111();

    int moveRandTunneling();
    int moveTowardTunneling(Coordinate coord);
};

char getSymbol(char type);

bool isInDun(Coordinate coord);
