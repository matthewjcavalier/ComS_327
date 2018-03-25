#ifndef STDLIB_H
  #define STDLIB_H
  #include <stdlib.h>
#endif
#ifndef VECTOR
  #define VECTOR
  #include <vector>
#endif

using namespace std;

class rollUp {
  public:
    int base;
    int diceType;
    int diceCount;
};

class monsterDesc {
  public:
    string name;
    string description;
    vector<string> colors;
    rollUp speed;
    vector<string> abilities;
    rollUp hp;
    rollUp ad;
    int rarity;
    char symbol;
};

class objectDesc {
  public:
    string name;
    string description;
    string type;
    vector<string> colors;
    rollUp hitBonus;
    rollUp damBonus;
    rollUp dodgeBonus;
    rollUp defenseBonus;
    rollUp weight;
    rollUp speedBuff;
    rollUp specialAttr;
    rollUp value;
    bool isArtifact;
    int rarity;
};