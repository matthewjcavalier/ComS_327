#include "Snake.hh"

Snake::Snake(Coordinate head) {
    length = 1;
    direction = UP;
    segments.push_back(head);
    draw();
}

void Snake::draw() {
    GameScreen* sc = GameScreen::Instance();
    attron(COLOR_PAIR(SNAKE));
    for(Coordinate segment : segments) {
        sc->drawCharacter(new Coordinate(segment.getY() + 1, segment.getX() + 1), '_');
    }
    attroff(COLOR_PAIR(SNAKE));
    refresh();
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
    Coordinate head = getHead();
    sc->drawEmpty({segments.back().getY() + 1, segments.back().getX() + 1});
    segments.pop_back();

    switch(direction) {
        case UP:
            if(true) {
                Coordinate newHead(head.getY() - 1, head.getX());
                segments.insert(segments.begin(), newHead);
            }
            break;
        case DOWN:
            break;
        case LEFT:
            break;
        case RIGHT:
            break;
    }
    draw();
    return 0;
}

Coordinate Snake::getHead() {
    return segments.front();
}