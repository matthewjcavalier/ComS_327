#include "Screen.hh"
#include <iostream>
using namespace std;

#define HEIGHT 30
#define WIDTH 80

GameScreen* GameScreen::_instance = 0;
GameScreen* GameScreen::Instance() {
    if(_instance == 0) {
        _instance = new GameScreen(HEIGHT,WIDTH);
    }
    return _instance;
}

GameScreen::GameScreen(int height, int width) {
    this->height = height;
    this->width = width;

    appleLoc.setX(rand() % width - 1);
    appleLoc.setY(rand() % height - 1);

    initscr();
    timeout(0);
    curs_set(FALSE);
    start_color();
    setupColors();
}

GameScreen::~GameScreen() {
    endwin();
}

int GameScreen::getHeight() {
    return height;
}

int GameScreen::getWidth() {
    return width;
}

bool GameScreen::isInScreenArea(Coordinate loc) {
    return (loc.getX() >= 0) && (loc.getX() < width) &&
           (loc.getY() >= 0) && (loc.getY() < height);
}

bool GameScreen::isInScreenArea(Coordinate* loc) {
   Coordinate temp(loc);
   return isInScreenArea(temp);
}

void GameScreen::drawScreen() {
    for(int y = 0; y < getHeight() + 1; y++) {
        for(int x = 0; x < getWidth() + 1; x++) {
            if(x==0 || y==0) {
                if(x==0)
                    drawCharacter(new Coordinate(y,0),  '0' + y%10);
                else
                    drawCharacter(new Coordinate(0,x),  '0' + x%10);
            }
            else if(y == 0 || y == getHeight() || x == 0 || x == getWidth()) {
                attron(COLOR_PAIR(BORDER));
                drawCharacter(new Coordinate(y,x), 'O');
                attroff(COLOR_PAIR(BORDER));
            } else {
                attron(COLOR_PAIR(EMPTY));
                drawCharacter(new Coordinate(y,x), '_');
                attroff(COLOR_PAIR(EMPTY));
            }
        }
    }
    drawApple();
    refresh();
}

void GameScreen::drawCharacter(Coordinate* loc, char c) {
    move(loc->getY(), loc->getX());
    addch(c);
}

void GameScreen::drawApple() {
    attron(COLOR_PAIR(APPLE));
    drawCharacter(&appleLoc, '_');
    attroff(COLOR_PAIR(APPLE));
}

void GameScreen::drawEmpty(Coordinate loc) {
    attron(COLOR_PAIR(EMPTY));
    drawCharacter(&loc, '_');
    attroff(COLOR_PAIR(EMPTY));
}

void GameScreen::setupColors() {
    init_pair(EMPTY, COLOR_WHITE, COLOR_WHITE);
    init_pair(BORDER, COLOR_BLACK, COLOR_BLACK);
    init_pair(APPLE, COLOR_RED, COLOR_RED);
    init_pair(SNAKE, COLOR_GREEN, COLOR_GREEN);
}