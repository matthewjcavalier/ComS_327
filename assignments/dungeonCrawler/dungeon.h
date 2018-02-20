#ifndef STDINT_H
  #define STDINT_H
  #include <stdint.h>
#endif

#define MAX_DUNGEON_HEIGHT 21
#define MAX_DUNGEON_WIDTH 80
#define MAX_ROCK_HARDNESS 255
#define MIN_ROCK_HARDNESS 0
#define MIN_ROOM_HEIGHT 2
#define MIN_ROOM_WIDTH 3
#define EXPECTED_ROOM_COUNT 10
#define MIN_ROOM_COUNT 5

#define INTELLIGENCE_BIT 0b0001
#define TELEPATHY_BIT 0b0010
#define TUNNELING_BIT 0b0100
#define ERRATIC_BIT 0b1000


typedef enum boolean {false, true} boolean;

typedef struct {
  uint8_t xPos;
  uint8_t yPos;
  uint8_t height;
  uint8_t width;
} Room;

typedef struct {
  uint8_t hardness;
  boolean isBorder;
  boolean isHallway;
  boolean isRoom;
} Tile;

typedef struct {
  Tile map[MAX_DUNGEON_HEIGHT][MAX_DUNGEON_WIDTH];
  struct List* rooms;
} Dungeon;

typedef struct Coordinate {
  int row;
  int col;
} Coordinate;

typedef struct Player {
  Coordinate coord;
} Player;

typedef struct NPC {
  char characteristics;
} NPC;

typedef struct Character {
  struct Coordinate coord;
  char symbol;
  struct Player* pc;
  struct NPC* npc;
  int speed;
  int nextEventTime;
} Character;
