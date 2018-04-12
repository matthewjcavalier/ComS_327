#include "Game.hh"

#define TIME_BETWEEN 250

int main(int argc, char* argv[]) {
    bool gameOver = false;
    srand(time(NULL));
    int userChar;
    long prevTime;
    long currTime;

    GameScreen* screen = GameScreen::Instance();
    screen->drawScreen();

    Snake snake({screen->getHeight() / 2, screen->getWidth() / 2});

    prevTime = getCurrentMilliseconds();
    while(!gameOver) {
        userChar = getch();
        screen->drawCharacter(new Coordinate(50,50), (char)userChar);
        setSnakeDir(userChar, snake);
        currTime = getCurrentMilliseconds();
        if(currTime - prevTime > TIME_BETWEEN) {
            snake.moveForward();
            prevTime = currTime;
        }
    }
    return 0;
}

void setSnakeDir(int userChar, Snake& snake) {
    switch(userChar) {
        case 'w':
            snake.setDirection(UP);
            break;
        case 'a':
            snake.setDirection(LEFT);
            break;
        case 's':
            snake.setDirection(DOWN);
            break;
        case 'd':
            snake.setDirection(RIGHT);
            break;
    }
}

long getCurrentMilliseconds() {
    return duration_cast< milliseconds >(system_clock::now().time_since_epoch()).count();
}