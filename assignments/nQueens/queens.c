#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
  int nQueens = 8; // number of queens to place


  // check for input args
  if(argc > 1) {
    // the user want's to do a custom n Queens
    nQueens = atoi(argv[1]);
  }

  return 0;
}
