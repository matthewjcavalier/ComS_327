CC=g++
CFLAGS=-g -Wall -Werror -std=c++11
REQ=clean genCtags game.cpp dungeon.cpp Character.cpp

all: $(REQ)
	$(CC) $(CFLAGS) game.cpp dungeon.cpp Character.cpp -o game
dungeon.o: dungeon.cpp
	$(CC) $(CFLAGS) dungeon.cpp -c
Character.o: Character.cpp
	$(CC) $(CFLAGS) Character.cpp -c
clean:
		- rm *.o
genCtags:
	ctags -R
run: all
	./game --nummon 1 --s 3
