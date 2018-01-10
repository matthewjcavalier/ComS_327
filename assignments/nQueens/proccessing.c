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
    return true;
}

bool noConflictLeftUpDiag(queen newPlacement, boardState currentBoard) {
    return true;
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
