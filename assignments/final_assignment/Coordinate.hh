
class Coordinate {
    public:
        Coordinate(int y, int x);
        Coordinate(Coordinate* loc);
        int getX();
        int getY();
        void setX(int x);
        void setY(int y);
        bool operator==(Coordinate comp);
    private:
        int x;
        int y;
};