#include "Game.hh"

#define TIME_BETWEEN 500

int main(int argc, char* argv[]) {
    bool gameOver = false;
    srand(time(NULL));
    //int userChar;
    long prevTime;
    long currTime;

    GameScreen* screen = GameScreen::Instance();
    screen->drawScreen();

    Snake snake({screen->getHeight() / 2, screen->getWidth() / 2});

    prevTime = getCurrentMilliseconds();
    while(!gameOver) {
//        userChar = getch();
        currTime = getCurrentMilliseconds();
        if(currTime - prevTime > TIME_BETWEEN) {
            snake.moveForward();
            prevTime = currTime;
        }
    }
    return 0;
}

long getCurrentMilliseconds() {
    return duration_cast< milliseconds >(system_clock::now().time_since_epoch()).count();
}