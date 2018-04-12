#include "Game.hh"

int main(int argc, char* argv[]) {
    bool gameOver = false;
    srand(time(NULL));

    GameScreen* screen = GameScreen::Instance();
    screen->drawScreen();


    while(!gameOver) {

    }
    return 0;
}