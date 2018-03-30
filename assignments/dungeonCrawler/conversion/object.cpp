#include "object.h"

object::object() {}

objectFactory::objectFactory() {}

object objectFactory::buildObject(objectDesc desc) {
  object newObj;
  newObj.name = desc.name;
  newObj.description = desc.description;
  newObj.type = desc.type;
  newObj.colors = desc.colors;
  newObj.hitBonus = rollDice(desc.hitBonus);
  newObj.damBonus = rollDice(desc.damBonus);
  newObj.dodgeBonus = rollDice(desc.dodgeBonus);
  newObj.defenseBonus = rollDice(desc.defenseBonus);
  newObj.weight = rollDice(desc.weight);
  newObj.speedBuff = rollDice(desc.speedBuff);
  newObj.specialAttr = rollDice(desc.specialAttr);
  newObj.value = rollDice(desc.value);

  return newObj;
}

int rollDice(rollUp fruit) {
  int total = fruit.base;
  for(int die = 0; die < fruit.diceCount; die++) {
    total += (rand() % fruit.diceType) + 1;
  }
  return total;
}
