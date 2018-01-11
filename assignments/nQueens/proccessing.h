// Libraries
#include <stdbool.h>
#include <stdlib.h>

#define MAX_BOARD_SIZE 8

// datatypes
typedef struct queen {
  int row;
  int col;
} queen;

typedef struct boardState{
  queen placements[MAX_BOARD_SIZE];
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
  int solutionsFound;
  solutionList list;
} solutions;


// prototypes

/**
 * @brief checks to see if the new placment of a queen violates the 
 *        no takeing rule in the right down diagonal direction
 * 
 * @param newPlacement the new location that a queen could be put at
 * @param currentBoard the current board state
 * @return true if the rule is not violated
 * @return false if the rule is violated
 */
bool noConflictRightDownDiag(queen newPlacement, boardState currentBoard);
/**
 * @brief checks to see if the new placment of a queen violates the 
 *        no takeing rule in the left down diagonal direction
 * 
 * @param newPlacement the new location that a queen could be put at
 * @param currentBoard the current board state
 * @return true if the rule is not violated
 * @return false if the rule is violated
 */
bool noConflictLeftDownDiag(queen newPlacement, boardState currentBoard);
/**
 * @brief checks to see if the new placment of a queen violates the 
 *        no takeing rule in the right up diagonal direction
 * 
 * @param newPlacement the new location that a queen could be put at
 * @param currentBoard the current board state
 * @return true if the rule is not violated
 * @return false if the rule is violated
 */
bool noConflictRightUpDiag(queen newPlacement, boardState currentBoard);
/**
 * @brief checks to see if the new placment of a queen violates the 
 *        no takeing rule in the left up diagonal direction
 * 
 * @param newPlacement the new location that a queen could be put at
 * @param currentBoard the current board state
 * @return true if the rule is not violated
 * @return false if the rule is violated
 */
bool noConflictLeftUpDiag(queen newPlacement, boardState currentBoard);
/**
 * @brief checks to see if the new placment of a queen violates the 
 *        no takeing rule on the vertical axis
 * 
 * @param newPlacement the new location that a queen could be put at
 * @param currentBoard the current board state
 * @return true if the rule is not violated
 * @return false if the rule is violated
 */
bool noConflictVertically(queen newPlacement, boardState currentBoard);
/**
 * @brief checks to see if the new placment of a queen violates the 
 *        no takeing rule on the horizontal axis
 * 
 * @param newPlacement the new location that a queen could be put at
 * @param currentBoard the current board state
 * @return true if the rule is not violated
 * @return false if the rule is violated
 */
bool noConflictHorizontally(queen newPlacement, boardState currentBoard);
/**
 * @brief checks to see if the newPlacement of a queen doesn't violate
 *        any of the no taking rules
 * 
 * @param newPlacement the new locaiton that a queen could be put at
 * @param currentBoard the current board state
 * @return true if none of the rules are violated
 * @return false if any of the rules are violated
 */
bool isValidPlacement(queen newPlacement, boardState currentBoard);
/**
 * @brief creates a list of strings that represent the solutions to
 *        the nQueens where the input nQueens is how many queens
 *        ther will be and the dimensions of the board they will
 *        be placed upon
 * 
 * @param nQueens 
 * @return char** 
 */
char** getSolutions(int nQueens);
/**
 * @brief Adds a new board state to the list that is pointed to
 * 
 * @param list the list to add to
 * @param newState the new state that is to be added
 */
void listAdd(solutionList* list, boardState newState);
/**
 * @brief Creates a new node for the list data structure
 * 
 * @param state the board state to be the data in the node
 * @return listNode* the pointer to the new node
 */
listNode* makeNode(boardState state);
/**
 * @brief generates an empty board that has MAX_BOARD_SIZE
 *        number of queens in it that all have their row and
 *        column locaitons set to -1
 * 
 * @return boardState the empty board
 */
boardState getEmptyBoard();
/**
 * @brief intializes a solutions list and returns it
 * 
 * @return solutionList 
 */
solutionList initList();
/**
 * @brief generates the solutions to the board
 * 
 * @param boardState current board state
 * @param sol solutions struct
 */
void placeNextPiece(boardState, solutions* sol);
char** convertSolutionsToStrings(solutions sol);