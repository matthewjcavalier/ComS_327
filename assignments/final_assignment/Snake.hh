#ifndef VECTOR
    #define VECTOR
    #include <vector>
#endif
#ifndef COORDINATE_HH
    #define COORDINATE_HH
    #include "Coordinate.hh"
#endif
#ifndef SCREEN_HH
    #define SCREEN_HH
    #include "Screen.hh"
#endif
#ifndef STDLIB_H
    #define STDLIB_H
    #include <stdlib.h>
#endif

using namespace std;

enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class Snake {
    public:
        Snake(Coordinate head);
        int getLength();
        vector<Coordinate> getSegments();
        Coordinate getSegment(int index);
        void setDirection(char direction);
        char getDirection();
        int moveForward();
        void draw();
        Coordinate getHead();
    private:
        int length;
        char direction;
        vector<Coordinate> segments;
        bool canMoveTo(Coordinate loc);
        bool isOccupied(Coordinate loc);
        void tryToEatApple(Coordinate loc);
        Coordinate getNewLoc();
};