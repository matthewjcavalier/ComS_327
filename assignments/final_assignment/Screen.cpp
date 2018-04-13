#include "Screen.hh"
#include <iostream>
#define SCREEN_DEBUG false

using namespace std;

#define SLOWEST_SPEED 100 

int _screen_width;
int _screen_height;

Settings::Settings() {
    float factor = 0.20;
    playAreaHeight = (int) _screen_height * factor;
    playAreaWidth = (int) _screen_width * factor;
    type = SCR_SMALL;
    spdVariant = SPD_LOW;
    speed = SLOWEST_SPEED - (int) (factor * spdVariant);
}

void Widget::draw() {
    // draw upper and lower bounds
    for(int x = coord.getX(); x < coord.getX() + width; x++) {
        move(coord.getY(), x);
        addch(ACS_HLINE);
        move(coord.getY() + height, x);
        addch(ACS_HLINE);
    }
    // draw side bounds
    for(int y = coord.getY(); y < coord.getY() + height; y++) {
        move(y, coord.getX());
        addch(ACS_VLINE);
        move(y, coord.getX() + width);
        addch(ACS_VLINE);
    }
    move(coord.getY(), coord.getX());
    addch(ACS_ULCORNER);
    move(coord.getY(), coord.getX() + width);
    addch(ACS_URCORNER);
    move(coord.getY() + height, coord.getX());
    addch(ACS_LLCORNER);
    move(coord.getY() + height, coord.getX() + width);
    addch(ACS_LRCORNER);
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
    setUpWelcomeScreen();
    setUpSettingsScreen();
    drawScreen(WELCOME);
    refresh();
}

void StartScreen::setUpWelcomeScreen() {
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
}

void StartScreen::setUpSettingsScreen() {
    Widget* widget = new Widget();
    int stdHeight = 3;
    widget->height = 12;
    widget-> width = 20;
    widget->coord.setY((_screen_height - widget->height)/2);
    widget->coord.setX((_screen_width - widget->width) / 2);

    Button* b = new Button();
    b->text = "Back";
    b->width = b->text.length() + 2;
    b->height = stdHeight;
    b->coord.setY(widget->coord.getY() + 1);
    b->coord.setX((widget->coord.getX() + (widget->width - b->width) / 2));

    widget->buttons.push_back(b);

    b = new Button();
    b->text = "Size: Small";
    b->width = b->text.length() + 2;
    b->height = stdHeight;
    b->coord.setY(widget->coord.getY() + stdHeight + 2);
    b->coord.setX((widget->coord.getX() + (widget->width - b->width) / 2));

    widget->buttons.push_back(b);

    b = new Button();
    b->text = "Granny Speed";
    b->width = b->text.length() + 2;
    b->height = stdHeight;
    b->coord.setY(widget->coord.getY() + stdHeight * 2 + 3);
    b->coord.setX((widget->coord.getX() + (widget->width - b->width) / 2));

    widget->buttons.push_back(b);


    widgets.push_back(widget);
}

void StartScreen::drawScreen(char index) {
    clearScreen();
    widgets[index]->draw();
}

Settings StartScreen::startUpRoutine() {
    char position = 0;
    bool updateHappend = true;
    char userPressed;
    while(true) {
        userPressed = getch();
        if(userPressed == '\033') {
            getch();
            userPressed = getch();
            if(userPressed == 'B' && position < (int)widgets[WELCOME]->buttons.size() - 1) {
                position++;
                updateHappend = true;
            } else if(userPressed == 'A' && position > 0) {
                position--;
                updateHappend = true;
            } 
        }
        else if(userPressed == 10) {
            if(position == 0) {
                clearScreen();
                return settings;
            } else if(position == 1) {
                runSettingsRoutine();
                updateHappend = true;
            }
        }
        if(updateHappend) {
            drawScreen(WELCOME);
            updateHappend = false;
            widgets[WELCOME]->buttons[position]->drawSelected();
        }
    }
}

void StartScreen::runSettingsRoutine() {
    clearScreen();
    drawScreen(SETTINGS);
    char position = 0;
    bool updateHappend = true;
    char userPressed;
    bool notDone = true;
    while(notDone) {
        userPressed = getch();
        if(userPressed == '\033') {
            getch();
            userPressed = getch();
            if(userPressed == 'B' && position < (int)widgets[SETTINGS]->buttons.size() - 1) {
                position++;
                updateHappend = true;
            } else if(userPressed == 'A' && position > 0) {
                position--;
                updateHappend = true;
            } 
        }
        else if(userPressed == 10) {
            if(position == SETTINGS_BACK) {
                notDone = false;
            } else if(position == SETTINGS_SIZE) {
                cycleThroughSizeOptions();
                updateHappend = true;
            } else if(position == SETTINGS_SPEED) {
                cycleThroughSpeedOptions();
                updateHappend = true;
            }
        }
        if(updateHappend) {
            drawScreen(SETTINGS);
            updateHappend = false;
            widgets[SETTINGS]->buttons[position]->drawSelected();
        }
    }
}

void StartScreen::cycleThroughSizeOptions() {
    double sizeFactor = .15;
    if(settings.type == SCR_INSANE){
        settings.type = SCR_SMALL;
    } else {
        settings.type++;
    }
    Button* b = widgets[SETTINGS]->buttons[SETTINGS_SIZE];
    sizeFactor *= (1 + settings.type);
    switch(settings.type) {
        case SCR_SMALL:
            b->text = "Size: Small";
            break;
        case SCR_MEDIUM:
            b->text = "Size: Medium";
            break;
        case SCR_LARGE:
            b->text = "Size: Large";
            break;
        case SCR_INSANE:
            b->text = "Size: Insane";
            sizeFactor = 1;
    }
    b->width = b->text.length() + 2;
    b->coord.setX(widgets[SETTINGS]->coord.getX() + (widgets[SETTINGS]->width - b->width) / 2);
    settings.playAreaHeight = (int) _screen_height * sizeFactor;
    settings.playAreaWidth = (int) _screen_width * sizeFactor;

    clearScreen();
    drawScreen(SETTINGS);
}

void StartScreen::cycleThroughSpeedOptions() {
    float factor = .10;
    if(settings.spdVariant == SPD_EXT){
        settings.spdVariant = SPD_LOW;
    } else {
        settings.spdVariant++;
    }
    Button* b = widgets[SETTINGS]->buttons[SETTINGS_SPEED];
    factor *= (settings.spdVariant);
    switch(settings.spdVariant) {
        case SPD_LOW:
            b->text = "Granny Speed";
            break;
        case SPD_MED:
            b->text = "Normal Slither";
            break;
        case SPD_HIGH:
            b->text = "Speedy Gonzales";
            break;
        case SPD_EXT:
            b->text = "Ludicrous Speed";
    }
    b->width = b->text.length() + 2;
    b->coord.setX(widgets[SETTINGS]->coord.getX() + (widgets[SETTINGS]->width - b->width) / 2);
    settings.speed = SLOWEST_SPEED - (int) (SLOWEST_SPEED * factor * settings.spdVariant);

    clearScreen();
    drawScreen(SETTINGS);   
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
    attron(COLOR_PAIR(BORDER));
    for(int x = 0; x < _screen_width; x++) {
        for(int y = 0; y < _screen_height; y++) {
            move(y,x);
            addch(' ');
        }
    }
    attroff(COLOR_PAIR(BORDER));
    refresh();
}