#include "Game.hh"

int main(int argc, char* argv[]) {
    bool gameOver = false;
    int height = 30;
    int width = 80;
    Coordinate appleLoc;
    srand(time(NULL));

    appleLoc.setX(rand() % width);
    appleLoc.setY(rand() % height);

    GameScreen* screen = GameScreen::Instance();
    screen->setHeight(height);
    screen->setWidth(width);
    screen->drawScreen();

    placeApple(appleLoc);

    while(!gameOver) {

    }
    return 0;
}

void placeApple(Coordinate loc) {
    GameScreen* sc = GameScreen::Instance();

    attron(COLOR_PAIR(APPLE));
    sc->drawCharacter(new Coordinate(loc.getY() + 1, loc.getX() + 1), '_');
    attroff(COLOR_PAIR(APPLE));
    refresh();
}