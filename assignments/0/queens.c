#include <stdio.h>

typedef enum  {false, true} boolean;


#define N_QUEENS 8

int board[N_QUEENS][N_QUEENS];

void clearBoard() {
    int row, col;
    
    for(row = 0; row < N_QUEENS; row++) {
        for(col = 0; col < N_QUEENS; col++) {
            board[row][col] = 0;
        }
    }
}

boolean invalidUpRight(int row, int col) {
    boolean invalid = false;
    row--;
    col++;

    while(row > -1 && col < N_QUEENS) {
        if(board[row][col] == 1) {
            invalid = true;
            break;
        }
        row--;
        col++;
    }

    return invalid;
}

boolean invlaidUpLeft(int row, int col) {
    boolean invalid = false;
    row--;
    col--;

    while(row > -1 && col > -1) {
        if(board[row][col] == 1) {
            invalid = true;
            break;
        }
        row--;
        col--;
    }

    return invalid;
}

boolean invalidDownRight(int row, int col) {
    boolean invalid = false;
    row++;
    col++;

    while(row < N_QUEENS && col < N_QUEENS) {
        if(board[row][col] == 1) {
            invalid = true;
            break;
        }
        row++;
        col++;
    }

    return invalid;
}

boolean invalidDownLeft(int row, int col) {
    boolean invalid = false;
    row++;
    col--;

    while(row < N_QUEENS && col > -1) {
        if(board[row][col] == 1) {
            invalid = true;
            break;
        }
        row++;
        col--;
    }

    return invalid;
}

boolean invalidVertically(int col) {
    boolean invalid = false;
    int row, queenCount = 0;
    
    for(row = 0; row < N_QUEENS; row++) {
        if(board[row][col] == 1) {
            queenCount++;
        }
    }

    if(queenCount > 1) {
        invalid = true;
    }

    return invalid;
}

boolean invalidHorizontally(int row) {
    boolean invalid = false;
    int col, queenCount = 0;

    for(col = 0; col < N_QUEENS; col++) {
        if(board[row][col] == 1) {
            queenCount++;
        }
    }

    if(queenCount > 1) {
        invalid = true;
    }

    return invalid;
}

boolean isValidPlacement(int row, int col) {
    boolean isValid = true;

    if(invalidUpRight(row, col)) {
        isValid = false;
    }
    if(invlaidUpLeft(row, col)) {
        isValid = false;
    }
    if(invalidDownRight(row, col)) {
        isValid = false;
    }
    if(invalidDownLeft(row, col)) {
        isValid = false;
    }
    if(invalidVertically(col)) {
        isValid = false;
    }
    if(invalidHorizontally(row)) {
        isValid = false;
    }

    return isValid;
}

void printBoardString() {
    int col, row;
    char charToPrint;
    for(col = 0; col < N_QUEENS; col++) {
        for(row = 0; row < N_QUEENS; row++) {
            if(board[row][col] == 1) {
                switch(col) {
                    case 0:
                        printf("a");
                        break;
                    case 1:
                        printf("b");
                        break;
                    case 2:
                        printf("c");
                        break;
                    case 3:
                        printf("d");
                        break;
                    case 4:
                        printf("e");
                        break;
                    case 5:
                        printf("f");
                        break;
                    case 6:
                        printf("g");
                        break;
                    case 7:
                        printf("h");
                        break;
                }
                printf("%d", row + 1);
            }
        }
    }
    printf("\n");
}

void placePeice(int row) {
    int col;
    
    for(col = 0; col < N_QUEENS; col++) {
        board[row][col] = 1;
        if(isValidPlacement(row, col)) {
            if(row == N_QUEENS - 1) {
                printBoardString();
            } else {
                placePeice(row + 1);
            }
        }
        board[row][col] = 0;
    }
    // reset the current row to be empty
    board[row][N_QUEENS - 1] = 0;
}

void findSolutions() {
    // clean the board
    clearBoard();
    
    placePeice(0);
}

int main(int argc, char* argv[]) {
    findSolutions();
    return 0;
}