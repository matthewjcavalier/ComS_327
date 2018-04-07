CC=g++
CFLAGS=-g -Wall -Werror -std=c++11
REQ=game.cpp dungeon.cpp Character.cpp screen.o fileReading.o
REQ1=genCtags game.cpp dungeon.cpp Character.cpp screen.o fileReading.o

all: $(REQ)
	$(CC) $(CFLAGS) game.cpp dungeon.cpp Character.cpp screen.o fileReading.o object.cpp monster.cpp -o game -lncurses
rebuild: clean all
dungeon.o: dungeon.cpp
	$(CC) $(CFLAGS) dungeon.cpp -c
Character.o: Character.cpp
	$(CC) $(CFLAGS) Character.cpp -c
screen.o: screen.cpp
	$(CC) $(CFLAGS) screen.cpp -c
dice.o: dice.cpp
	$(CC) $(CFLAGS) dice.cpp -c
diceTest: dice.cpp
	$(CC) $(CFLAGS) dice.cpp -o dice
fileReading.o: fileReading.cpp
	$(CC) $(CFLAGS) fileReading.cpp -c 
clean:
		- rm *.o
		- rm game
		- rm -rf test
		- rm *.tar.gz
genCtags:
	ctags -R
run: all
	./game --s 123 --nummon 1
exportLog:
	git --no-pager log > CHANGELOG
package: clean exportLog
	mkdir cavalier_matthew.assignment-1.08
	cp Character.* defs.h dungeon.* game.* Makefile screen.* fileReading.* desc.h object.* monster.* README CHANGELOG cavalier_matthew.assignment-1.08
	tar cvfz cavalier_matthew.assignment-1.08.tar.gz cavalier_matthew.assignment-1.08
	rm -rf cavalier_matthew.assignment-1.08
test: package
	mkdir test
	tar -xf cavalier_matthew.assignment-1.08.tar.gz -C test/
connect:
	ssh cavalier@pyrite.cs.iastate.edu
