CC=gcc
CFLAGS=-Wall -Werror -std=c99
OBJS=crawler.o generator.o list.o list.o pathFinding.o queue.o heap.o

all: crawler debug
crawler: $(OBJS)
	$(CC) $(CFLAGS) crawler.o generator.o list.o pathFinding.o queue.o heap.o -o crawler
debug: $(OBJS)
	$(CC) -g $(CFLAGS) crawler.o generator.o list.o pathFinding.o queue.o heap.o -o debug
crawler.o: crawler.c
	$(CC) -g $(CFLAGS) -c crawler.c
generator.o: generator.c
	$(CC) -g $(CFLAGS) -c generator.c
list.o: list.c
	$(CC) -g $(CFLAGS) -c list.c
pathFinding.o: pathFinding.c
	$(CC) -g $(CFLAGS) -c pathFinding.c
queue.o: queue.c
	$(CC) -g $(CFLAGS) -c queue.c
heap.o: heap.c 
	$(CC) -g $(CFLAGS) -c heap.c
heap: heap.o
	$(CC) -g $(CFLAGS) heap.o -o heap
run: clean crawler debug
	./crawler -cool --nummon 5
clean:
	- rm debug crawler
	- rm *.o
	- rm -rf test/
	- rm cavalier_matthew.assignment-1.03.tar.gz
	- rm valgrind_output.*
	- rm vgcore.*
add: clean
	git add .
exportLog:
	git --no-pager log > CHANGELOG
package: clean
	mkdir cavalier_matthew.assignment-1.03
	cp generator.* crawler.* list.* queue.* pathFinding.* dungeon.h Makefile README CHANGELOG cavalier_matthew.assignment-1.03
	tar cvfz cavalier_matthew.assignment-1.03.tar.gz cavalier_matthew.assignment-1.03/
	rm -rf cavalier_matthew.assignment-1.03
final: clean package
analyze: clean debug
	valgrind --log-file="valgrind_output.txt" --leak-check=full ./debug
test: package
	mkdir test
	tar -xf cavalier_matthew.assignment-1.03.tar.gz -C test/
connect:
	ssh cavalier@pyrite.cs.iastate.edu
