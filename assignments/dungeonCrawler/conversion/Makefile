CC=g++
CFLAGS=-g -Wall -Werror -std=c++11
REQ=clean genCtags dungeon.o game.cpp Character.o

all: $(REQ)
	$(CC) $(CFLAGS) game.cpp dungeon.o Character.o -o game
dungeon.o: dungeon.cpp
	$(CC) $(CFLAGS) dungeon.cpp -c
Character.o: Character.cpp
	$(CC) $(CFLAGS) Character.cpp -c
clean:
		- rm *.o
genCtags:
	ctags -R
run: all
	./game --load
