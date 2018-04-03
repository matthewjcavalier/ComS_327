#ifndef STDLIB_H
  #define STDLIB_H
  #include <stdlib.h>
#endif
#ifndef VECTOR
  #define VECTOR
  #include <vector>
#endif
#ifndef DESC_H
  #define DESC_H
  #include "desc.h"
#endif
#ifndef STRING
  #define STRING
  #include <string>
#endif
#ifndef CHARACTER_H
  #define CHARACTER_H
  #include "Character.h"
#endif
#ifndef FILEREADING_H
  #define FILEREADING_H
  #include "fileReading.h"
#endif
#ifndef DUNGEON_H
  #define DUNGEON_H
  #include "dungeon.h"
#endif


class monsterFactory {
  public: 
    Dungeon* dun;
    monsterFactory(Dungeon* dun);
    vector<NPC*> buildMonsters(vector<monsterDesc> descList, int numToBuild, int currentEventTime, PC* pc);
    void buildMonster(monsterDesc desc, NPC* monster, int currentEventTime, PC* pc);
};

char genType(vector<string> abilities);
int rollDice(rollUp fruit);