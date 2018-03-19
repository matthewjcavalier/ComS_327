CC=g++
CFLAGS=-g -Wall -Werror -std=c++11
REQ=clean genCtags game.cpp dungeon.cpp Character.cpp screen.o

all: $(REQ)
	$(CC) $(CFLAGS) game.cpp dungeon.cpp Character.cpp screen.o -o game -lncurses
dungeon.o: dungeon.cpp
	$(CC) $(CFLAGS) dungeon.cpp -c
Character.o: Character.cpp
	$(CC) $(CFLAGS) Character.cpp -c
screen.o: screen.cpp
	$(CC) $(CFLAGS) screen.cpp -c
clean:
		- rm *.o
		- rm game
		- rm -rf test
		- rm *.tar.gz
genCtags:
	ctags -R
run: all
	./game
exportLog:
	git --no-pager log > CHANGELOG
package: clean exportLog
	mkdir cavalier_matthew.assignment-1.06
	cp Character.* defs.h dungeon.* game.* Makefile screen.* README CHANGELOG cavalier_matthew.assignment-1.06
	tar cvfz cavalier_matthew.assignment-1.06.tar.gz cavalier_matthew.assignment-1.06
	rm -rf cavalier_matthew.assignment-1.06
test: package
	mkdir test
	tar -xf cavalier_matthew.assignment-1.06.tar.gz -C test/
connect:
	ssh cavalier@pyrite.cs.iastate.edu
