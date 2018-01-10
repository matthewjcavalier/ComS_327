// Libraries
#include <stdbool.h>

// datatypes
typedef struct queen {
  int row;
  int col;
} queen;

typedef struct boardState{
  queen placements[8];
  int numQueens;
  int boardSize;
} boardState;

typedef struct solutions {
  int boardSize;
  boardState solutionsArr[];
} soutions;

// prototypes
bool noConflictRightDownDiag(queen newPlacement, boardState currentBoard);

bool noConflictLeftDownDiag(queen newPlacement, boardState currentBoard);

bool noConflictRightUpDiag(queen newPlacement, boardState currentBoard);

bool noConflictLeftUpDiag(queen newPlacement, boardState currentBoard);

bool noConflictVertically(queen newPlacement, boardState currentBoard);

bool noConflictHorizontally(queen newPlacement, boardState currentBoard);

bool isValidPlacement(queen newPlacement, boardState currentBoard);
