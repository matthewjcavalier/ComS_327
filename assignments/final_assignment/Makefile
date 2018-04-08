CC=g++
CFLAGS=-g -Wall -Werror -std=c++11


all:

clean:
	- rm *.o
	- rm test_Coordinate
Coordinate.o: Coordinate.cpp
	$(CC) $(CFLAGS) Coordinate.cpp -c
test_Coordinate: Coordinate.o
	$(CC) $(CFLAGS) Coordinate.o -o test_Coordinate

Screen.o: Screen.cpp
	$(CC) $(CFLAGS) Screen.cpp -c
test_Screen: Screen.o Coordinate.o
	$(CC) $(CFLAGS) Screen.o Coordinate.o -o test_Screen