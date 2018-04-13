#include "Game.hh"

#define TIME_BETWEEN 100

int main(int argc, char* argv[]) {
    bool gameOver = false;
    //srand(time(NULL));
    srand(1);
    int userChar;
    long prevTime;
    long currTime;
    char lastDirMoved;
//    int score;
    screenSetup();
    GameScreen* screen = GameScreen::Instance(20,20);
    screen->drawScreen();

    Snake snake({screen->getHeight() / 2, screen->getWidth() / 2});
//    score = snake.getLength() * 150;

    prevTime = getCurrentMilliseconds();
    lastDirMoved = snake.getDirection();
    while(!gameOver) {
        userChar = getch();
        setSnakeDir(userChar, snake, lastDirMoved);
        currTime = getCurrentMilliseconds();
        if(currTime - prevTime > TIME_BETWEEN) {
            lastDirMoved = snake.getDirection();
            if(snake.moveForward()) {
                gameOver = true;
            }
            prevTime = currTime;
        }
    }
    delete screen;
    return 0;
}

void setSnakeDir(int userChar, Snake& snake, char lastDirMoved) {
    switch(userChar) {
        case 'w':
            if(snake.getDirection() != DOWN && lastDirMoved != DOWN) {
                snake.setDirection(UP);
            }
            break;
        case 'a':
            if(snake.getDirection() != RIGHT && lastDirMoved != RIGHT) {
                snake.setDirection(LEFT);
            }
            break;
        case 's':

            if(snake.getDirection() != UP && lastDirMoved != UP) {
                snake.setDirection(DOWN);
            }
            break;
        case 'd':
            if(snake.getDirection() != LEFT && lastDirMoved != LEFT) {
                snake.setDirection(RIGHT);
            }
            break;
    }
}

long getCurrentMilliseconds() {
    return duration_cast< milliseconds >(system_clock::now().time_since_epoch()).count();
}