#include "unity.h"
#include "proccessing.h"

void test_1_1(void) {
  int sizeOfBoard = 3;

  queen original;
  original.row = 1;
  original.col = 1;

  queen expected;
  expected.row = 1;
  expected.col = 3;

  queen actual = rotatePosition(original, sizeOfBoard);

  TEST_ASSERT_EQUAL(expected.row, actual.row);
  TEST_ASSERT_EQUAL(expected.col, actual.col);
}


void test_1_2(void) {
  int sizeOfBoard = 3;

  queen original;
  original.row = 1;
  original.col = 2;

  queen expected;
  expected.row = 2;
  expected.col = 3;

  queen actual = rotatePosition(original, sizeOfBoard);

  TEST_ASSERT_EQUAL(expected.row, actual.row);
  TEST_ASSERT_EQUAL(expected.col, actual.col);
}

void test_1_3(void) {
  int sizeOfBoard = 3;

  queen original;
  original.row = 1;
  original.col = 3;

  queen expected;
  expected.row = 3;
  expected.col = 3;

  queen actual = rotatePosition(original, sizeOfBoard);

  TEST_ASSERT_EQUAL(expected.row, actual.row);
  TEST_ASSERT_EQUAL(expected.col, actual.col);
}

void test_2_1(void) {
  int sizeOfBoard = 3;

  queen original;
  original.row = 2;
  original.col = 1;

  queen expected;
  expected.row = 1;
  expected.col = 2;

  queen actual = rotatePosition(original, sizeOfBoard);

  TEST_ASSERT_EQUAL(expected.row, actual.row);
  TEST_ASSERT_EQUAL(expected.col, actual.col);
}

void test_2_2(void) {
  int sizeOfBoard = 3;

  queen original;
  original.row = 2;
  original.col = 2;

  queen expected;
  expected.row = 2;
  expected.col = 2;

  queen actual = rotatePosition(original, sizeOfBoard);

  TEST_ASSERT_EQUAL(expected.row, actual.row);
  TEST_ASSERT_EQUAL(expected.col, actual.col);
}

void test_3_1(void) {
  int sizeOfBoard = 3;

  queen original;
  original.row = 3;
  original.col = 1;

  queen expected;
  expected.row = 1;
  expected.col = 1;

  queen actual = rotatePosition(original, sizeOfBoard);

  TEST_ASSERT_EQUAL(expected.row, actual.row);
  TEST_ASSERT_EQUAL(expected.col, actual.col);
}
int main(void) {
  RUN_TEST(test_1_1);
  RUN_TEST(test_1_2);
  RUN_TEST(test_1_3);
  RUN_TEST(test_2_1);
  RUN_TEST(test_2_2);
  RUN_TEST(test_3_1);
}
