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
        void setWidth(int width);
        void setHeight(int height);
        bool isInScreenArea(Coordinate loc);
        bool isInScreenArea(Coordinate* loc);
        void drawScreen();
        void drawCharacter(Coordinate* loc, char c);
    protected:
        GameScreen(int height, int width);
        ~GameScreen();
    private:
        int height;
        int width; 
        static GameScreen* _instance;
        void setupColors();
};