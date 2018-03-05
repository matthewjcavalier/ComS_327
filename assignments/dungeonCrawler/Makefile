CC=gcc
CFLAGS=-Wall -Werror -std=gnu99
OBJS=crawler.o generator.o list.o list.o pathFinding.o queue.o heap.o printer.o

all: clear crawler debug
crawler: $(OBJS)
	$(CC) $(CFLAGS) crawler.o generator.o list.o pathFinding.o queue.o heap.o printer.o -o crawler -lncursesw
debug: $(OBJS)
	- ctags -R
	$(CC) -g $(CFLAGS) crawler.o generator.o list.o pathFinding.o queue.o heap.o printer.o -o debug -lncursesw
clear:
		clear
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
printer.o: printer.c
	$(CC) -g $(CFLAGS) -c printer.c
printer: printer.o
	$(CC) -g $(CFLAGS) printer.o -o printer -lncurses
runOut: clean crawler debug
	./crawler --nummon 1 -s 1 > out.txt
run: clean crawler debug
	./crawler --nummon 1 -s 5157108739

clean:
	- rm debug crawler
	- rm *.o
	- rm -rf test/
	- rm cavalier_matthew.assignment-1.04.tar.gz
	- rm valgrind_output.*
	- rm vgcore.*
add: clean
	git add .
exportLog:
	git --no-pager log > CHANGELOG
package: clean
	mkdir cavalier_matthew.assignment-1.04
	cp heap.* generator.* crawler.* list.* queue.* pathFinding.* dungeon.h Makefile README CHANGELOG CMD_line_args.txt cavalier_matthew.assignment-1.04
	tar cvfz cavalier_matthew.assignment-1.04.tar.gz cavalier_matthew.assignment-1.04/
	rm -rf cavalier_matthew.assignment-1.04
final: clean package
analyze: clean debug
	valgrind --log-file="valgrind_output.txt" --leak-check=full ./debug
test: package
	mkdir test
	tar -xf cavalier_matthew.assignment-1.04.tar.gz -C test/
connect:
	ssh cavalier@pyrite.cs.iastate.edu
