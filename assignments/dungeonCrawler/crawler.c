#include <stdio.h>

#define MAX_DUNGEON_HEIGHT 21
#define MAX_DUNGEON_WIDTH 80


typedef struct {
  int hardness;
} rock;

int main(int argc, char* argv[]) {
  // set up the dungeon
  hardness dungeon[MAX_DUNGEON_HEIGHT][MAX_DUNGEON_WIDTH];

  dungeon = genDungeon();
  
  // print the dungeon
  
}
