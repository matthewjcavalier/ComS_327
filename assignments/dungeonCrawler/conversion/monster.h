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
  #include "character.h"
#endif

class monster: public NPC {
  public:
    string name;
    string description;
    vector<string> colors;
    int speed;
    vector<string> abilities;
    int hp;
    rollUp ad;
    int rarity;
    char symbol;
    monster();
};

class monsterFactory() {
  public: 
    monsterFactory();
    vector<monster> buildMonsters(vector<monsterDesc> descList);
    monster buildMonster(monsterDesc desc);
};
