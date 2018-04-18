CC=g++
CFLAGS=-g -Wall -Werror -std=c++11

all: clean Snake

run: all
	./Snake

Snake: Game.o Coordinate.o Screen.cpp Snake.cpp
	$(CC) $(CFLAGS) Game.o Coordinate.o Screen.cpp Snake.cpp -o Snake -lncurses

clean:
	- rm *.o
	- rm test_Coordinate
	- rm *.tar.gz
	- rm -rf test

Game.o: Game.cpp 
	$(CC) $(CFLAGS) Game.cpp -c


Coordinate.o: Coordinate.cpp
	$(CC) $(CFLAGS) Coordinate.cpp -c

Screen.o: Screen.cpp
	$(CC) $(CFLAGS) Screen.cpp -c


package: clean
	mkdir cavalier_matthew.assignment-1.10
	cp Makefile Coordinate.* Game.* Screen.* Snake.* statusCodes.hh cavalier_matthew.assignment-1.10
	tar cvfz cavalier_matthew.assignment-1.10.tar.gz cavalier_matthew.assignment-1.10
	rm -rf cavalier_matthew.assignment-1.10

test: package
	mkdir test
	tar -xf cavalier_matthew.assignment-1.10.tar.gz -C test/
connect:
	ssh cavalier@pyrite.cs.iastate.edu