#include "proccessing.h"

queen rotatePosition(queen input, int sizeOfBoard) {
  int oldrow = input.row;
  int oldcol = input.col;
  queen newLoc;

  newLoc.row = oldcol;
  newLoc.col = 3 - oldrow + 1;
  
  return newLoc;
}
