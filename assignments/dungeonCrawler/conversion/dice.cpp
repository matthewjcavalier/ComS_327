#ifndef STDLIB_H
  #define STDLIB_H
  #include <stdlib.h>
#endif
#ifndef VECTOR
  #define VECTOR
  #include <vector>
#endif
#include <iostream>

using namespace std;

class dice {
  public:
    int numSides;
    dice(int numSides) {
      this->numSides = numSides;
    }
    int roll() {
      return rand() % numSides + 1;
    }
    vector<int> roll(int numDice) {
      vector<int> results;
      for(int i = 0; i < numDice; i++) {
        results.push_back(roll());
      }
      return results;
    }
};

int main() {
  dice d20(20);
  vector<int> result;
  int counter = 0;
  do {
    result = d20.roll(3);
    cout << "rolling 3 d20" << endl;
    for(int res : result) {
      cout << res << endl;
    }
  } while(counter++ < 10);
  return 0;
}
