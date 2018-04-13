#ifndef COORDINATE_HH
    #define COORDINATE_HH
    #include "Coordinate.hh"
#endif
#ifndef NCURSES_H
    #define NCURSES_H
    #include <ncurses.h>
#endif

enum colorTypes{
    SNAKE,
    APPLE,
    BORDER,
    EMPTY
};

class GameScreen {
    public:
        static GameScreen* Instance();
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
        Coordinate appleLoc;
        static GameScreen* _instance;
        void setupColors();
};