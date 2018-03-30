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


using namespace std;

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
    objectFactory();
    vector<object> buildObjects(vector<objectDesc> descList);
    object buildObject(objectDesc desc);
};

int rollDice(rollUp fruit);
