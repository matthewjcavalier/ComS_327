#ifndef SCREEN_HH
    #define SCREEN_HH
    #include "Screen.hh"
#endif
#ifndef COORDINATE_HH
    #define COORDINATE_HH
    #include "Coordinate.hh"
#endif
#ifndef STDLIB_H
    #define STDLIB_H
    #include <stdlib.h>
#endif
#ifndef TIME_H
    #define TIME_H
    #include <time.h>
#endif
#ifndef SNAKE_HH
    #define SNAKE_HH
    #include "Snake.hh"
#endif
#ifndef CHRONO
    #define CHRONO
    #include <chrono>
#endif
#ifndef UNISTD_H
    #define UNISTD_H
    #include <unistd.h>
#endif
#ifndef STDIO_H
    #define STDIO_H
    #include <stdio.h>
#endif

using namespace std::chrono;

long getCurrentMilliseconds();

void setSnakeDir(int userChar, Snake& snake, char lastDirMoved);