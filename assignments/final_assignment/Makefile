CC=g++
CFLAGS=-g -Wall -Werror -std=c++11


all: clean Snake

Snake: Game.o Coordinate.o Screen.o
	$(CC) $(CFLAGS) Game.o Coordinate.o Screen.o -o Snake -lncurses

clean:
	- rm *.o
	- rm test_Coordinate

Game.o: Game.cpp 
	$(CC) $(CFLAGS) Game.cpp -c


Coordinate.o: Coordinate.cpp
	$(CC) $(CFLAGS) Coordinate.cpp -c

Screen.o: Screen.cpp
	$(CC) $(CFLAGS) Screen.cpp -c
