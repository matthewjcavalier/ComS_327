#include "object.h"

/**
 * @brief Construct a new object::object object
 * 
 */
object::object() {}

/**
 * @brief Construct a new object Factory::object Factory object
 * 
 * @param dun 
 */
objectFactory::objectFactory(Dungeon* dun) {
  this->dun = dun;
}

/**
 * @brief Builds objects using the input list of descriptions and puts them into the dungeon
 * 
 * @param descList 
 * @param numToBuild 
 * @return int 
 */
int objectFactory::buildObjects(vector<objectDesc>& descList, int numToBuild) {
  int builtCount = 0;
  bool noDescSelected = true;
  objectDesc desc;

  while(builtCount < numToBuild) {
    // select a description from the list
    while(noDescSelected) {
      int index = rand() % descList.size();
      int randInt = rand() % 100;
      if(descList[index].rarity > randInt) {
        if(descList[index].isArtifact && descList[index].wasCreated == false) {
          noDescSelected = false;
          desc = descList[index];
          descList[index].wasCreated = true;
        } else if(descList[index].isArtifact == false) {
          noDescSelected = false;
          desc = descList[index];
        }
      }
    }
    object* obj = new object();
    buildObject(desc, obj);
    Coordinate loc = dun->getEmptySpace();
    dun->objectMap[loc.y][loc.x] = obj;
    noDescSelected = true;
    builtCount++;
  }
  return 0;
}

/**
 * @brief builds a single object
 * 
 * @param desc 
 * @param obj 
 * @return int 
 */
int objectFactory::buildObject(objectDesc desc, object* obj) {
  obj->name = desc.name;
  obj->description = desc.description;
  obj->type = desc.type;
  obj->colors = desc.colors;
  obj->hitBonus = rollDice(desc.hitBonus);
  obj->damBonus = rollDice(desc.damBonus);
  obj->dodgeBonus = rollDice(desc.dodgeBonus);
  obj->defenseBonus = rollDice(desc.defenseBonus);
  obj->weight = rollDice(desc.weight);
  obj->speedBuff = rollDice(desc.speedBuff);
  obj->specialAttr = rollDice(desc.specialAttr);
  obj->value = rollDice(desc.value);
  obj->symbol = getObjSym(obj->type);

  return 0;
}

/**
 * @brief Get the Obj Sym character
 * 
 * @param type 
 * @return char 
 */
char getObjSym(string type) {
  if(type.compare("WEAPON") == 0) {return WEAPON_SYM;}
  else if(type.compare("OFFHAND") == 0) {return OFFHAND_SYM;}
  else if(type.compare("RANGED") == 0) {return RANGED_SYM;}
  else if(type.compare("ARMOR") == 0) {return ARMOR_SYM;}
  else if(type.compare("HELMET") == 0) {return HELMET_SYM;}
  else if(type.compare("CLOAK") == 0) {return CLOAK_SYM;}
  else if(type.compare("GLOVES") == 0) {return GLOVES_SYM;}
  else if(type.compare("BOOTS") == 0) {return BOOTS_SYM;}
  else if(type.compare("RING") == 0) {return RING_SYM;}
  else if(type.compare("AMULET") == 0) {return AMULET_SYM;}
  else if(type.compare("LIGHT") == 0) {return LIGHT_SYM;}
  else if(type.compare("SCROLL") == 0) {return SCROLL_SYM;}
  else if(type.compare("BOOK") == 0) {return BOOK_SYM;}
  else if(type.compare("FLASK") == 0) {return FLASK_SYM;}
  else if(type.compare("GOLD") == 0) {return GOLD_SYM;}
  else if(type.compare("AMMUNITION") == 0) {return AMMUNITION_SYM;}
  else if(type.compare("FOOD") == 0) {return FOOD_SYM;}
  else if(type.compare("WAND") == 0) {return WAND_SYM;}
  else if(type.compare("CONTAINER") == 0) {return CONTAINER_SYM;}
  else if(type.compare("STACK") == 0) {return STACK_SYM;}
  return '*';
}
