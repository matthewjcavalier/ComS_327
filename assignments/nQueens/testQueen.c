#include "unity.h"
#include "proccessing.h"

boardState genBoard() {
    boardState currentState;

    queen firstQ;
    firstQ.row = 2;
    firstQ.col = 2;

    queen secondQ;
    secondQ.row = 4;
    secondQ.col = 3;

    currentState.placements[0] = firstQ;
    currentState.placements[1] = secondQ;

    currentState.numQueens = 2;
    currentState.boardSize = 6;

    return currentState;
}

void test_validPlacement() {
    boardState currentState = genBoard();

    queen newQueen;
    newQueen.row = 1;
    newQueen.col = 4;
    bool result = isValidPlacement(newQueen, currentState);
    TEST_ASSERT_TRUE(result);
}

void test_invalidPlacement_same_row() {
    boardState currentState = genBoard();

    queen newQueen;
    newQueen.row = 2;
    newQueen.col = 1;

    TEST_ASSERT_FALSE(isValidPlacement(newQueen, currentState));
}

void test_invalidPlacement_same_col() {
    boardState currentState = genBoard();

    queen newQueen;
    newQueen.row = 0;
    newQueen.col = 3;

    TEST_ASSERT_FALSE(isValidPlacement(newQueen, currentState));
}

void test_invalidPlacement_right_down_violation() {
    boardState currentState = genBoard();

    queen newQueen;
    newQueen.row = 0;
    newQueen.col = 0;

    TEST_ASSERT_FALSE(isValidPlacement(newQueen, currentState));
}

void test_invalidPlacement_left_down_violation() {
    boardState currentState = genBoard();

    queen newQueen;
    newQueen.row = 0;
    newQueen.col = 4;

    TEST_ASSERT_FALSE(isValidPlacement(newQueen, currentState));
}

void test_invalidPlacement_right_up_violation() {
    boardState currentState = genBoard();

    queen newQueen;
    newQueen.row = 3;
    newQueen.col = 1;

    TEST_ASSERT_FALSE(isValidPlacement(newQueen, currentState));   
}

void test_invalidPlacement_left_up_violation() {
    boardState currentState = genBoard();

    queen newQueen;
    newQueen.row = 5;
    newQueen.col = 5;

    TEST_ASSERT_FALSE(isValidPlacement(newQueen, currentState));   
}


int main(void) {
    RUN_TEST(test_validPlacement);
    RUN_TEST(test_invalidPlacement_same_row);
    RUN_TEST(test_invalidPlacement_same_col);
    RUN_TEST(test_invalidPlacement_right_down_violation);
    RUN_TEST(test_invalidPlacement_left_down_violation);
    RUN_TEST(test_invalidPlacement_right_up_violation);
    RUN_TEST(test_invalidPlacement_left_up_violation);
}
