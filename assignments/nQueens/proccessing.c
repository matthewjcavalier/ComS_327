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
    boardState newBoard;
    queen firstQueen;
    solutions boardSolutions;

    firstQueen.row = 0;
    firstQueen.col = 0;

    newBoard.numQueens = 1;
    newBoard.boardSize = 1;

    newBoard.placements[0] = firstQueen;

    boardSolutions.boardSize = 1;
    boardSolutions.list = initList();
    
    listAdd(&boardSolutions.list, newBoard);

    int x;
    x = 3 + 4;
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

//solutions getNextSolutionSet()