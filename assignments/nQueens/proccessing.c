#include "proccessing.h"


bool noConflictRightDownDiag(queen newPlacement, boardState currentBoard) {
    bool ret = true;
    int row, col, queen;

    row = newPlacement.row;
    col = newPlacement.col;

    while(row < currentBoard.boardSize && col < currentBoard.boardSize) {
        for(queen = 0; queen < currentBoard.numQueens; queen++) {
            if(currentBoard.placements[queen].row == row &&
                currentBoard.placements[queen].col == col) {
                    ret = false;
                    break;
            }
        }
        row++;
        col++;
    }

    return ret;
}

bool noConflictLeftDownDiag(queen newPlacement, boardState currentBoard) {
    bool ret = true;
    int row, col, queen;

    row = newPlacement.row;
    col = newPlacement.col;

    while(row < currentBoard.boardSize && col > -1) {
        for(queen = 0; queen < currentBoard.numQueens; queen++) {
            if(currentBoard.placements[queen].row == row &&
                currentBoard.placements[queen].col == col) {
                    ret = false;
                    break;
            }
        }
        row++;
        col--;
    }

    return ret;
}

bool noConflictRightUpDiag(queen newPlacement, boardState currentBoard) {
    bool ret = true;
    int row, col, queen;

    row = newPlacement.row;
    col = newPlacement.col;

    while(row > -1 && col < currentBoard.boardSize) {
        for(queen = 0; queen < currentBoard.numQueens; queen++) {
            if(currentBoard.placements[queen].row == row &&
                currentBoard.placements[queen].col == col) {
                    ret = false;
                    break;
            }
        }
        row--;
        col++;
    }

    return ret;
}

bool noConflictLeftUpDiag(queen newPlacement, boardState currentBoard) {
    bool ret = true;
    int row, col, queen;

    row = newPlacement.row;
    col = newPlacement.col;

    while(row > -1 && col > -1) {
        for(queen = 0; queen < currentBoard.numQueens; queen++) {
            if(currentBoard.placements[queen].row == row &&
                currentBoard.placements[queen].col == col) {
                    ret = false;
                    break;
            }
        }
        row--;
        col--;
    }

    return ret;
}

bool noConflictVertically(queen newPlacement, boardState currentBoard) {
    bool ret = true;
    int i;
    for(i = 0; i < currentBoard.numQueens; i++) {
       if(currentBoard.placements[i].col == newPlacement.col) {
           ret = false;
           break;
       } 
    }
    return ret;
}

bool noConflictHorizontally(queen newPlacement, boardState currentBoard) {
    bool ret = true;
    int i;
    for(i = 0; i < currentBoard.numQueens; i++) {
       if(currentBoard.placements[i].row == newPlacement.row) {
           ret = false;
           break;
       } 
    }
    return ret;
}

bool isValidPlacement(queen newPlacement, boardState currentBoard) {
    bool ret = false;
    if(noConflictRightDownDiag(newPlacement, currentBoard) &&
       noConflictLeftDownDiag(newPlacement, currentBoard) &&
       noConflictRightUpDiag(newPlacement, currentBoard) &&
       noConflictLeftUpDiag(newPlacement, currentBoard) &&
       noConflictVertically(newPlacement, currentBoard) &&
       noConflictHorizontally(newPlacement, currentBoard)) {
           ret = true;
       }
    return ret;
}

char** getSolutions(int nQueens) {
    boardState newBoard = getEmptyBoard();
    queen firstQueen;
    solutions boardSolutions;
    bool solutionExists;
    int i;

    firstQueen.row = 0;
    firstQueen.col = 0;

    newBoard.numQueens = 1;
    newBoard.boardSize = 1;

    newBoard.placements[0] = firstQueen;

    boardSolutions.boardSize = 1;
    boardSolutions.list = initList();
    boardSolutions.solutionsFound = 1;
    
    listAdd(&boardSolutions.list, newBoard);
    for(i = 1; i < nQueens; i++) {
       solutionExists = getNextSolutionSet(&boardSolutions, i);
    }
}

solutionList initList() {
    solutionList list;
    list.size = 0;
    list.head = NULL;

    return list;
}

void listAdd(solutionList* list, boardState newState) {
    listNode* newNode = makeNode(newState);
    listNode* currentNode;
    int i;

    // if the list is empty
    if(list->size == 0) {
        list->size = 1;
        list->head = newNode;
    }
    // else the list has entries
    else {
        // getting last node in list
        currentNode = list->head;
        for(i = 1; i < list->size; i++) {
            currentNode = currentNode->next;
        }
        currentNode->next = newNode;
        list->size = list->size + 1;
    }
}

listNode* makeNode(boardState state) {
    listNode node;
    listNode* nodePtr = malloc(sizeof(listNode));

    node.data = state;
    node.next = NULL;

    *nodePtr = node;
    return nodePtr;
}

boardState getEmptyBoard() {
    boardState newBoard;
    queen impossibleQueen;
    int i;

    newBoard.numQueens = 0;
    newBoard.boardSize = 0;

    impossibleQueen.row = -1;
    impossibleQueen.col = -1;

    for(i = 0; i < MAX_BOARD_SIZE; i++) {
        newBoard.placements[i] = impossibleQueen;
    }

    return newBoard;
}

bool getNextSolutionSet(solutions* boardSolutions, int boardSize) {
    int size = boardSolutions->boardSize;
    int col, row, i;
    listNode* currentNode;
    solutions newSolutions;
    boardState currentSolution;
    boardState newSolutionBoard;
    queen newQueen;
    int queenIttr;
    bool newSolutionFound = false;

    currentNode = boardSolutions->list.head;

    // loop through the existing solutions to the n-1 problem
    for(i = 0; i < boardSolutions->solutionsFound; i++) {
        currentSolution = currentNode->data;

        // loop through the last column of the new board
        newQueen.col = boardSize;
        for(row = 0; row < boardSize + 1; row++) {
            newQueen.row = row;
            if(isValidPlacement(newQueen, currentSolution)) {
                newSolutionBoard.numQueens = boardSize;
                newSolutionBoard.boardSize = boardSize;
                for(queenIttr = 0; queenIttr < boardSize; queenIttr++) {
                    newSolutionBoard.placements[queenIttr] = currentSolution.placements[queenIttr];
                }
                newSolutionBoard.placements[boardSize] = newQueen;
                // if this is the first new solution
                if(!newSolutionFound) {
                    // make a new solutions list
                    newSolutions.solutionsFound = 0;
                    newSolutions.boardSize = boardSize;
                    newSolutions.list = initList();
                    newSolutionFound = true;
                }
                listAdd(&newSolutions.list, newSolutionBoard);
                newSolutions.solutionsFound++;
            }
        }
        // loop through the last row of the board excpet for the last column of the last row
        newQueen.row = boardSize;
        // this only itterates to boardsize since we have already checked the last column
        for(col = 0; col < boardSize; col++) {
            newQueen.col = col;
            if(isValidPlacement(newQueen, currentSolution)) {
                newSolutionBoard.numQueens = boardSize;
                newSolutionBoard.boardSize = boardSize;
                for(queenIttr = 0; queenIttr < boardSize; queenIttr++) {
                    newSolutionBoard.placements[queenIttr] = currentSolution.placements[queenIttr];
                }
                newSolutionBoard.placements[boardSize] = newQueen;
                // if this is the first new solution
                if(!newSolutionFound) {
                    // make a new solutions list
                    newSolutions.solutionsFound = 0;
                    newSolutions.boardSize = boardSize;
                    newSolutions.list = initList();
                    newSolutionFound = true;
                }
                listAdd(&newSolutions.list, newSolutionBoard);
                newSolutions.solutionsFound++;
            }
        }
        currentNode = currentNode->next;
    }

    if(newSolutionFound) {
        *boardSolutions = newSolutions;
    }
    return newSolutionFound;
}