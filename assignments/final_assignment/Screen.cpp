#include "Screen.hh"
#include <iostream>
using namespace std;

GameScreen* GameScreen::_instance = 0;
GameScreen* GameScreen::Instance() {
    if(_instance == 0) {
        _instance = new GameScreen(0,0);
    }
    return _instance;
}

GameScreen::GameScreen(int height, int width) {
    this->height = height;
    this->width = width;
}

void GameScreen::setWidth(int width) {
    this->width = width;
}

void GameScreen::setHeight(int height) {
    this->height = height;
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

int main(int argc, char* argv[]) {
    GameScreen* screen = GameScreen::Instance();
    screen->setHeight(10);
    screen->setWidth(12);
    Coordinate c1(4,4);

    cout<<screen->getHeight()<<endl;
    cout<<screen->getWidth()<<endl;
    cout<<screen->isInScreenArea(Coordinate(4,4))<<endl;
    cout<<screen->isInScreenArea(new Coordinate(15,15))<<endl;

    return 0;
}