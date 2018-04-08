#include "Coordinate.hh"

Coordinate::Coordinate(Coordinate* loc) {
    Coordinate(loc->y, loc->x);
}

Coordinate::Coordinate(int y, int x) {
    this->y = y;
    this->x = x;
}

Coordinate::Coordinate() {
    y = x = -1;
}

int Coordinate::getX() {
    return x;
}

int Coordinate::getY() {
    return y;
}

void Coordinate::setX(int x) {
    this->x = x;
}

void Coordinate::setY(int y) {
    this->y = y;
}

bool Coordinate::operator==(Coordinate comp) {
    return (x == comp.x) && (y == comp.y);
}