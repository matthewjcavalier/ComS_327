#ifndef STDIO_H
  #define STDIO_H
  #include <stdio.h>
#endif
#ifndef STDLIB_H
  #define STDLIB_H
  #include <stdlib.h>
#endif
#ifndef STRING_H
  #define STRING_H
  #include <string.h>
#endif
#ifndef LIST_H
  #define LIST_H
  #include "list.h"
#endif
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


typedef enum {false, true} boolean;

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

/**
 * @brief   Performs the dungeon generatation steps to 
 *          create a new dungeon from scratch
 * 
 * @return Dungeon* 
 */
Dungeon* genDungeon();

/**
 * @brief   Cleans the dungeon and sets all tiles in the
 *          map to a starting state
 * 
 * @param dun   the dungeon being worked on
 */
void initDungeon(Dungeon* dun);

/**
 * @brief   Adds a border to the dungeon. Each tile in
 *          the border has its isBorder flag turned on.
 * 
 * @param dun   the dungeon being worked on
 */
void addBorders(Dungeon* dun);

/**
 * @brief   Iterates throught he dungeon and sets the
 *          hardness of all tiles to something in the
 *          range 0 to 255.
 * @param dun   the dungeon being worked on
 */
void setHardnesses(Dungeon* dun);

/**
 * @brief   Places between 5 to 10 rooms in the dungeon.
 * 
 * @param dun   the dungeon being worked on
 */
void placeRooms(Dungeon* dun);

/**
 * @brief   Cleans the dungeon rooms list as well as removes
 *          the placed rooms from the dungeon.
 * 
 * @param dun   the dungeon being worked on
 */
void clearRooms(Dungeon* dun);

/**
 * @brief   Checks to see if placing the newRoom at the coordinates
 *          violates any room placement rules.
 * 
 * @param xLoc      the x coordinate for the new room
 * @param yLoc      the y coordinate for the new room 
 * @param dun       a pointer to the dungeon being worked on 
 * @param newRoom   the new room that is looking to be placed
 * @return boolean  if the placment is valid true is returned, else
 *                  false is returned
 */
boolean isValidRoomPlacement(int xLoc, int yLoc, Dungeon* dun, Room newRoom);

/**
 * @brief   Adds the new room to the dungeon.
 * 
 * @param dun       the dungeon being worked on
 * @param newRoom   the new room that needs placement
 */
void addRoomToDungeon(Dungeon* dun, Room newRoom); 

/**
 * @brief   Places hallways in the dungeon ensuring that
 *          all rooms are connected in some fashion.
 * 
 * @param dun   the dungeon being worked on
 */
void placeHallways(Dungeon* dun);

/**
 * @brief   makes a path from the location pointed to by the
 *          first set of coordinates to the second set of
 *          coordinates placing hallway tiles down as it
 *          moves between the two.
 * 
 * @param row1  the y location for the first coordinate pair
 * @param col1  the x locaiton for the first coordinate pair
 * @param row2  the y location for the second coordinate pair
 * @param col2  the x location for the second coordinate pair
 * @param dun   the dungeon being worked on
 */
void makePathToRoom(int row1, int col1, int row2, int col2, Dungeon* dun);

/**
 * @brief   attempts to place a halltile at the givine coordinate,
 *          if there is found to be a room tile already present
 *          at that coordinate, then the tile is not updated
 * 
 * @param row   the y location for the coordinate pair
 * @param col   the x locaiton for the coordinate pair
 * @param dun   the dungeon being worked on
 */
void placeHallTile(int row, int col, Dungeon* dun);

/**
 * @brief Saves the current dungeon to the location
 *        specified by saveLoc
 * 
 * @param dun       the dungeon to save
 * @param saveLoc   where to save the dungeon
 */
void saveDungeon(Dungeon* dun, char* saveLoc);

/**
 * @brief Loads and builds a dungeon bassed on
 *        the location pointed to by LoadLoc
 * 
 * @param LoadLoc     where to load the new dungeon from
 * @return Dungeon*   a pointer to the new dungeon
 */
Dungeon* loadDungeon(char* LoadLoc);

/**
 * @brief Swaps the endian ness of the input 32 bit
 *        vairable
 * 
 * @param input       the variable to swap
 * @return uint32_t   the swapped value
 */
uint32_t endianSwap_uInt(uint32_t input);

/**
 * @brief Goes through the dungeon defining what
 *        each tile is
 * 
 * @param dun  the dungeon to define tiles in
 */
void defineTiles(Dungeon* dun);
