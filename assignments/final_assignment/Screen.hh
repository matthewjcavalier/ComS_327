#ifndef CORDINATE_HH
    #define CORDINATE_HH
    #include "Coordinate.hh"
#endif

class GameScreen {
    public:
        static GameScreen* Instance();
        int getWidth();
        int getHeight();
        void setWidth(int width);
        void setHeight(int height);
        bool isInScreenArea(Coordinate loc);
        bool isInScreenArea(Coordinate* loc);
    protected:
        GameScreen(int height, int width);
    private:
        int height;
        int width; 
        static GameScreen* _instance;
};