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
#ifndef DUNGEON_H
  #define DUNGEON_H
  #include "dungeon.h"
#endif
#ifndef FILEREADING_H
  #define FILEREADING_H
  #include "fileReading.h"
#endif


#ifndef OBJECT_H_BUILT
#define OBJECT_H_BUILT

using namespace std;

class Dungeon;

class object {
  public:
    string name;
    string description;
    string type;
    vector<string> colors;
    int hitBonus;
    int damBonus;
    int dodgeBonus;
    int defenseBonus;
    int weight;
    int speedBuff;
    int specialAttr;
    int value;
    bool isArtifact;

    char symbol;

    object();
};

class objectFactory {
  public:
    Dungeon* dun;
    objectFactory(Dungeon* dun);
    int buildObjects(vector<objectDesc>& descList, int numToBuild);
    int buildObject(objectDesc desc, object* obj);
};

char getObjSym(string type);

#endif
