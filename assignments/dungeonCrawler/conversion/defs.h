#define MAX_HARDNESS 255
#define MAX_WIDTH 80
#define MAX_HEIGHT 21
#define MIN_ROOM_HEIGHT 2 
#define MIN_ROOM_WIDTH 3
#define MIN_ROOM_COUNT 5

#define INTELLIGENCE_BIT 0b0001
#define TELEPATHY_BIT 0b0010
#define TUNNELING_BIT 0b0100
#define ERRATIC_BIT 0b1000

class Coordinate {
  public:
    int x;
    int y;
    Coordinate() {
      x = 0;
      y = 0;
    }
    Coordinate(int y, int x) {
      this->y = y;
      this->x = x;
    }
};
