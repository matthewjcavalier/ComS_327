#ifndef COORDINATE_HH
    #define COORDINATE_HH
    #include "Coordinate.hh"
#endif
#ifndef NCURSES_H
    #define NCURSES_H
    #include <ncurses.h>
#endif
#ifndef VECTOR
    #define VECTOR
    #include <vector>
#endif
#ifndef STRING 
    #define STRING
    #include <string>
#endif

using namespace std;

enum colorTypes{
    BLUE,
    SNAKE1,
    SNAKE2,
    APPLE,
    BORDER,
    EMPTY,
    BUTTON_SEL,
    BUTTON_SEL_TEXT,
    BUTTON,
    BUTTON_TEXT
};

enum Screens {
    WELCOME,
    SETTINGS
};

enum ScreenSizeTypes{
    SCR_SMALL,
    SCR_MEDIUM,
    SCR_LARGE,
    SCR_INSANE
};

class Settings {
    public:
        int playAreaWidth;
        int playAreaHeight;
        char type;
        Settings();
};

class Button {
    public:
        Coordinate coord;
        string text;
        int width;
        int height;
        void draw();
        void drawSelected();
};

class Widget {
    public:
        Coordinate coord;
        int width;
        int height;
        vector<Button*> buttons;
        void draw();
};

class StartScreen {
    public:
        StartScreen();
        Settings startUpRoutine();
        vector<Widget*> widgets;
    private:
        Settings settings;
        void setUpWelcomeScreen();
        void setUpSettingsScreen();
        void drawScreen(char index);
        void runSettingsRoutine();
        void cycleThroughSizeOptions();
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

void clearScreen();