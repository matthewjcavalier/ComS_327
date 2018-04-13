#include "Screen.hh"
#include <iostream>
#define SCREEN_DEBUG false

using namespace std;

int _screen_width;
int _screen_height;

Settings::Settings() {
    playAreaWidth = 80;
    playAreaHeight = 30;
}

void Widget::draw() {
    // draw upper and lower bounds
    for(int x = coord.getX(); x < coord.getX() + width; x++) {
        move(coord.getY(), x);
        addch(ACS_HLINE);
        move(coord.getY() + height, x);
        addch(ACS_HLINE);
    }
    refresh();
    for(Button* b : buttons) {
        b->draw();
    }
}

void Button::draw() {
    attron(COLOR_PAIR(BUTTON));
    for(int y = coord.getY(); y < coord.getY() + height; y++) {
        for(int x = coord.getX(); x < coord.getX() + width; x++) {
            move(y,x);
            addch('_');
            refresh();
        }
    }
    attroff(COLOR_PAIR(BUTTON));

    refresh();
    attron(COLOR_PAIR(BUTTON_TEXT));
    mvprintw(coord.getY() + 1, coord.getX() + 1, text.c_str());
    attroff(COLOR_PAIR(BUTTON_TEXT));
    refresh();
}

void Button::drawSelected() {
    attron(COLOR_PAIR(BUTTON_SEL));
    for(int y = coord.getY(); y < coord.getY() + height; y++) {
        for(int x = coord.getX(); x < coord.getX() + width; x++) {
            move(y,x);
            addch('_');
        }
    }
    attroff(COLOR_PAIR(BUTTON_SEL));

    refresh();
    attron(COLOR_PAIR(BUTTON_SEL_TEXT));
    mvprintw(coord.getY() + 1, coord.getX() + 1, text.c_str());
    attroff(COLOR_PAIR(BUTTON_SEL_TEXT));
    refresh();
}

StartScreen::StartScreen() {
    Widget* widget = new Widget();

    widget->height = 10;
    widget-> width = 20;
    widget->coord.setY((_screen_height - widget->height)/2);
    widget->coord.setX((_screen_width - widget->width) / 2);

    Button* b = new Button();
    b->text = "Start Game";
    b->width = b->text.length() + 2;
    b->height = 3;
    b->coord.setY(widget->coord.getY() + 1);
    b->coord.setX((widget->coord.getX() + (widget->width - b->width) / 2));

    widget->buttons.push_back(b);

    b = new Button();
    b->text = "Settings";
    b->width = b->text.length() + 2;
    b->height = 3;
    b->coord.setY(widget->coord.getY() + widget->height - b->height);
    b->coord.setX((widget->coord.getX() + (widget->width - b->width) / 2));

    widget->buttons.push_back(b);

    widgets.push_back(widget);

    drawInitialScreen();
    refresh();
}

void StartScreen::drawInitialScreen() {
        widgets[0]->draw();
}

Settings StartScreen::startUpRoutine() {
    char position = 0;
    bool updateHappend = true;
    char userPressed;
    while(true) {
        userPressed = getch();
        if(userPressed == 's' && position < (int)widgets[0]->buttons.size() - 1) {
            position++;
            updateHappend = true;
        } else if(userPressed == 'w' && position > 0) {
            position--;
            updateHappend = true;
        } else if(userPressed == 10) {
            if(position == 0) {
                return settings;
            } else if(position == 1) {
                runSettingsRoutine();
            }
        }
        if(updateHappend) {
            drawInitialScreen();
            updateHappend = false;
            widgets[0]->buttons[position]->drawSelected();
        }
    }
}

void StartScreen::runSettingsRoutine() {
    clearScreen();
    
}

GameScreen* GameScreen::_instance = 0;
GameScreen* GameScreen::Instance() {
    if(_instance == 0) {
        _instance = new GameScreen(30,80);
    }
    return _instance;
}

GameScreen* GameScreen::Instance(int height, int width) {
    if(_instance == 0) {
        _instance = new GameScreen(height, width);
    }
    return _instance;
}

GameScreen::GameScreen(int height, int width) {
    this->height = height;
    this->width = width;
    do {
        appleLoc.setX(rand() % width - 1);
        appleLoc.setY(rand() % height - 1);
    } while(!isInScreenArea(appleLoc));
    v_offset = (_screen_height - height) / 2;
    h_offset = (_screen_width - width) / 2;
    screenSetup();
}

GameScreen::~GameScreen() {
    endwin();
}

int GameScreen::getHeight() {
    return height;
}

int GameScreen::getWidth() {
    return width;
}

bool GameScreen::isInScreenArea(Coordinate loc) {
    bool ret = (loc.getX() >= 0) && (loc.getX() < width - 1) &&
           (loc.getY() >= 0) && (loc.getY() < height - 1);
    return ret;
}

bool GameScreen::isInScreenArea(Coordinate* loc) {
   Coordinate temp(loc);
   return isInScreenArea(temp);
}

void GameScreen::drawScreen() {
    for(int y = 0; y < getHeight() + 1; y++) {
        for(int x = 0; x < getWidth() + 1; x++) {
            if(SCREEN_DEBUG) {
                if(x==0 || y==0) {
                    if(x==0)
                        drawCharacter(new Coordinate(y,0),  '0' + (y-1)%10);
                    else
                        drawCharacter(new Coordinate(0,x),  '0' + (x-1)%10);
                }
            }
            else if(y == 0 || y == getHeight() || x == 0 || x == getWidth()) {
                attron(COLOR_PAIR(BORDER));
                drawCharacter(new Coordinate(y,x), 'O');
                attroff(COLOR_PAIR(BORDER));
            } else {
                attron(COLOR_PAIR(EMPTY));
                drawCharacter(new Coordinate(y,x), '_');
                attroff(COLOR_PAIR(EMPTY));
            }
        }
    }
    drawApple();
    refresh();
}

void GameScreen::drawCharacter(Coordinate* loc, char c) {
    move(loc->getY() + v_offset, loc->getX() + h_offset);
    addch(c);
}

void GameScreen::drawApple() {
    attron(COLOR_PAIR(APPLE));
    drawCharacter(new Coordinate(appleLoc.getY() + 1, appleLoc.getX() + 1), '_');
    attroff(COLOR_PAIR(APPLE));
}

Coordinate GameScreen::getAppleLoc() {
    return appleLoc;
}

void GameScreen::setAppleLoc(Coordinate loc) {
    appleLoc = loc;
}

void GameScreen::drawEmpty(Coordinate loc) {
    attron(COLOR_PAIR(EMPTY));
    drawCharacter(&loc, '_');
    attroff(COLOR_PAIR(EMPTY));
}

void setupColors() {
    init_pair(EMPTY, COLOR_WHITE, COLOR_WHITE);
    init_pair(BORDER, COLOR_BLACK, COLOR_BLACK);
    init_pair(APPLE, COLOR_RED, COLOR_RED);
    init_pair(SNAKE1, COLOR_GREEN, COLOR_BLACK);
    init_pair(SNAKE2, COLOR_BLACK, COLOR_GREEN);
    init_pair(BUTTON, COLOR_GREEN, COLOR_GREEN);
    init_pair(BUTTON_SEL, COLOR_WHITE, COLOR_WHITE);
    init_pair(BUTTON_TEXT, COLOR_WHITE, COLOR_GREEN);
    init_pair(BUTTON_SEL_TEXT, COLOR_GREEN, COLOR_WHITE);
}

int screenSetup() {
    static bool setup = false;
    if(!setup) {
        setup = true;
        WINDOW * win = initscr();
        timeout(0);
        curs_set(FALSE);
        noecho();
        start_color();
        setupColors();
        getmaxyx(win, _screen_height, _screen_width);
    }
    return 0;
}

void clearScreen() {
    for(int x = 0; x < _screen_width; x++) {
        for(int y = 0; y < _screen_height; y++) {
            move(y,x);
            addch(' ');
        }
    }
}