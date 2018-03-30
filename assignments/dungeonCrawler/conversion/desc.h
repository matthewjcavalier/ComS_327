#ifndef STDLIB_H
  #define STDLIB_H
  #include <stdlib.h>
#endif
#ifndef VECTOR
  #define VECTOR
  #include <vector>
#endif
#ifndef STRING
  #define STRING
  #include <string>
#endif

#define WEAPON_SYM '|'
#define OFFHAND_SYM ')'
#define RANGED_SYM '}'
#define ARMOR_SYM '['
#define HELMET_SYM ']'
#define CLOAK_SYM '('
#define GLOVES_SYM '{'
#define BOOTS_SYM '\\'
#define RING_SYM '='
#define AMULET_SYM '"'
#define LIGHT_SYM '_'
#define SCROLL_SYM '~'
#define BOOK_SYM '?'
#define FLASK_SYM '!'
#define GOLD_SYM '$'
#define AMMUNITION_SYM '/'
#define FOOD_SYM ','
#define WAND_SYM '-'
#define CONTAINER_SYM '%'
#define STACK_SYM '&'

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
