
class Character {
  public:
    int x;
    int y;
    char symbol;
    int speed;
    int nextEventTime;
    Dungeon dun;

    Character();
    Character(Dungeon& dun, int x, int y, char symbol, int speed, int nextEventTime);
    void takeTurn();
};
