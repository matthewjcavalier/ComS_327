#ifndef STDLIB_H
  #define STDLIB_H
  #include <stdlib.h>
#endif
#ifndef DESC_H
  #define DESC_H
  #include "desc.h"
#endif

using namespace std;

int rollDice(rollUp fruit) {
  int total = fruit.base;

  if(fruit.diceCount == 0 || fruit.diceType == 0) {
    return fruit.base;
  }

  for(int die = 0; die < fruit.diceCount; die++) {
    total += (rand() % fruit.diceType) + 1;
  }
  return total;
}
