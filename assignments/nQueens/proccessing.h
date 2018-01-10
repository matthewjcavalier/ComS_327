// Libraries
#include <stdbool.h>
#include <stdlib.h>

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

typedef struct listNode {
  boardState data;
  struct listNode* next;
} listNode;

typedef struct solutionList {
  int size;
  struct listNode* head;
} solutionList;

typedef struct solutions {
  int boardSize;
  solutionList list;
  int solutionsFound;
} solutions;

// prototypes
bool noConflictRightDownDiag(queen newPlacement, boardState currentBoard);

bool noConflictLeftDownDiag(queen newPlacement, boardState currentBoard);

bool noConflictRightUpDiag(queen newPlacement, boardState currentBoard);

bool noConflictLeftUpDiag(queen newPlacement, boardState currentBoard);

bool noConflictVertically(queen newPlacement, boardState currentBoard);

bool noConflictHorizontally(queen newPlacement, boardState currentBoard);

bool isValidPlacement(queen newPlacement, boardState currentBoard);

char** getSolutions(int nQueens);

solutionList initList();

void listAdd(solutionList* list, boardState newState);

listNode* makeNode(boardState state);