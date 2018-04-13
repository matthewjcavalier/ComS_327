#include "Game.hh"

#define TIME_BETWEEN 100

int main(int argc, char* argv[]) {
    bool gameOver = false;
    //srand(time(NULL));
    srand(1);
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
            if(snake.moveForward()) {
                gameOver = true;
            }
            prevTime = currTime;
        }
    }
    delete screen;
    return 0;
}

void setSnakeDir(int userChar, Snake& snake) {
    switch(userChar) {
        case 'w':
            if(snake.getDirection() != DOWN) {
                snake.setDirection(UP);
            }
            break;
        case 'a':
            if(snake.getDirection() != RIGHT) {
                snake.setDirection(LEFT);
            }
            break;
        case 's':

            if(snake.getDirection() != UP) {
                snake.setDirection(DOWN);
            }
            break;
        case 'd':
            if(snake.getDirection() != LEFT) {
                snake.setDirection(RIGHT);
            }
            break;
    }
}

long getCurrentMilliseconds() {
    return duration_cast< milliseconds >(system_clock::now().time_since_epoch()).count();
}