#ifndef COORDINATE_HH
    #define COORDINATE_HH
    #include "Coordinate.hh"
#endif
#ifndef NCURSES_H
    #define NCURSES_H
    #include <ncurses.h>
#endif

enum colorTypes{
    BLUE,
    SNAKE1,
    SNAKE2,
    APPLE,
    BORDER,
    EMPTY
};

class GameScreen {
    public:
        static GameScreen* Instance();
        static GameScreen* Instance(int height, int width);
        int getWidth();
        int getHeight();
        bool isInScreenArea(Coordinate loc);
        bool isInScreenArea(Coordinate* loc);
        void drawScreen();
        void drawCharacter(Coordinate* loc, char c);
        void drawEmpty(Coordinate loc);
        void drawApple();
        Coordinate getAppleLoc();
        void setAppleLoc(Coordinate loc);
        ~GameScreen();
    protected:
        GameScreen(int height, int width);
    private:
        int height;
        int width; 
        int v_offset;
        int h_offset;
        Coordinate appleLoc;
        static GameScreen* _instance;
};

int screenSetup();
void setupColors();