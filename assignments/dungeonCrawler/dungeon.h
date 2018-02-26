#ifndef STDINT_H
  #define STDINT_H
  #include <stdint.h>
#endif
#ifndef CURSES_H
  #define CURSES_H
  #include <curses.h>
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

#define BORDER_HOR_CHAR '-'
#define BORDER_VERT_CHAR '|'
#define ROOM_CHAR '.'
#define HALL_CHAR '#'
#define PC_CHAR '@'

//#define COOL_ROOM_CHAR            "\u26c6"  //  ⛆ 
#define COOL_ROOM_CHAR            "\u2593"  //  ▓
//#define COOL_HALL_CHAR            "\u26da"  //  ⛚
#define COOL_HALL_CHAR            "\u2592"  //  ▒
#define COOL_ROCK                 "\u2591"  //  ░
#define COOL_BORDER_TOP_LEFT      "\u2554"  // 	╔
#define COOL_BORDER_TOP_RIGHT     "\u2557"  //  ╗
#define COOL_BORDER_BOTTOM_LEFT   "\u255a"  //  ╚
#define COOL_BORDER_BOTTOM_RIGHT  "\u255d"  //  ╝
#define COOL_BORDER_VERT          "\u2551"  //  ║
#define COOL_BORDER_HOR           "\u2550"  //  ═

typedef struct {
  uint8_t xPos;
  uint8_t yPos;
  uint8_t height;
  uint8_t width;
} Room;

typedef struct Setup{
  int seed;
  bool useCoolChars;
  bool save;
  bool load;
  char* saveLoadLocation;
  int numMonsters;
} Setup;

typedef struct {
  uint8_t hardness;
  bool isBorder;
  bool isHallway;
  bool isRoom;
  bool isUpstairs;
  bool isDownstairs;
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
} Player;

typedef struct NPC {
  char characteristics;
  Coordinate lastKnowPCLoc;
  bool hasSeenPC;
} NPC;

typedef struct Character {
  struct Coordinate coord;
  char symbol;
  struct Player* pc;
  struct NPC* npc;
  int speed;
  int nextEventTime;
} Character;
