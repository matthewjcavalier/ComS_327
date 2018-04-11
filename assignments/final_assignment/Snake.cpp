#include "Snake.hh"

Snake::Snake(Coordinate head) {
    length = 1;
    direction = UP;
    segments.push_back(head);
}

int Snake::getLength() {
    return length;
}

vector<Coordinate> Snake::getSegments() {
    return segments;
}

Coordinate Snake::getSegment(int index) {
    return segments[index];
}

void Snake::setDirection(char direction) {
    this->direction = direction;
}

int Snake::moveForward() {
    GameScreen* sc = GameScreen::Instance();
    switch(direction) {
        case UP:
            break;
        case DOWN:
            break;
        case LEFT:
            break;
        case RIGHT:
            break;
    }
    return 0;
}