#include "Snake.hh"

Snake::Snake(Coordinate head) {
    length = 1;
    direction = RIGHT;
    for(int i = 0; i < 9; i++) {
        segments.push_back(head);
    }
    draw();
}

void Snake::draw() {
    GameScreen* sc = GameScreen::Instance();
    int count = 1;

    for(Coordinate segment : segments) {
        (count++ % 2 == 0) ? attron(COLOR_PAIR(SNAKE1)) : attron(COLOR_PAIR(SNAKE2));
        sc->drawCharacter(new Coordinate(segment.getY() + 1, segment.getX() + 1), '#');
        attroff(COLOR_PAIR(SNAKE1));
        attroff(COLOR_PAIR(SNAKE2));
    }
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

char Snake::getDirection() {
    return direction;
}

int Snake::moveForward() {
    GameScreen* sc = GameScreen::Instance();
    Coordinate head = getHead();
    sc->drawEmpty({segments.back().getY() + 1, segments.back().getX() + 1});
    segments.pop_back();

    switch(direction) {
        case UP: {
            Coordinate newHead(head.getY() - 1, head.getX());
            if(canMoveTo(newHead)) {
                tryToEatApple(newHead);
                segments.insert(segments.begin(), newHead);
            } else {
                return 1;
            }
            break;
        }
        case DOWN: {
            Coordinate newHead(head.getY() + 1, head.getX());
            if(canMoveTo(newHead)) {
                tryToEatApple(newHead);
                segments.insert(segments.begin(), newHead);
            } else {
                return 1;
            }
            break;
        }
        case LEFT: {
            Coordinate newHead(head.getY(), head.getX() - 1);
            if(canMoveTo(newHead)) {
                tryToEatApple(newHead);
                segments.insert(segments.begin(), newHead);
            } else {
                return 1;
            }
            break;
        }
        case RIGHT: {
            Coordinate newHead(head.getY(), head.getX() + 1);
            if(canMoveTo(newHead)) {
                tryToEatApple(newHead);
                segments.insert(segments.begin(), newHead);
            } else {
                return 1;
            }
            break;
        }
    }
    draw();
    return 0;
}

void Snake::tryToEatApple(Coordinate loc) {
    GameScreen* gs = GameScreen::Instance();
    if(loc == gs->getAppleLoc()) {
        segments.push_back(loc);
        gs->setAppleLoc(getNewLoc());
        gs->drawApple();
        refresh();
    }
}

Coordinate Snake::getNewLoc() {
    Coordinate loc;
    GameScreen* gs = GameScreen::Instance();
    bool isPlaced = false;
    bool inArea;
    bool isOCC;
    do {
        loc.setX(rand() % gs->getHeight() - 1);
        loc.setY(rand() % gs->getWidth() -1);
        inArea = gs->isInScreenArea(loc);
        isOCC = isOccupied(loc);
        if(inArea && !isOCC) {
            isPlaced = true;
        }
    } while(isPlaced == false);
    return loc;
}

Coordinate Snake::getHead() {
    return segments.front();
}

bool Snake::canMoveTo(Coordinate loc) {
    GameScreen* sc = GameScreen::Instance();
    return sc->isInScreenArea(loc) && !isOccupied(loc);
}

bool Snake::isOccupied(Coordinate loc) {
    for(auto segment : segments) {
        if(segment == loc) {
            return true;
        }
    }
    return false;
}