CC=gcc
CFLAGS=-Wall -Werror
OBJS=crawler.o generator.o list.o list.o pathFinding.o queue.o

all: crawler debug
crawler: $(OBJS)
	$(CC) $(CFLAGS) crawler.o generator.o list.o pathFinding.o queue.o -o crawler
debug: $(OBJS)
	$(CC) -g $(CFLAGS) crawler.o generator.o list.o pathFinding.o queue.o -o debug
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
run: clean crawler
	./crawler -cool 
clean:
	- rm debug crawler
	- rm *.o
	- rm -rf test/
	- rm cavalier_matthew.assignment-1.02.tar.gz
	- rm valgrind_output.*
	- rm vgcore.*
add: clean
	git add .
exportLog:
	git --no-pager log > CHANGELOG
package: clean
	mkdir cavalier_matthew.assignment-1.02
	cp generator.* crawler.* list.* queue.* pathFinding.* dungeon.h Makefile README CHANGELOG cavalier_matthew.assignment-1.02
	tar cvfz cavalier_matthew.assignment-1.02.tar.gz cavalier_matthew.assignment-1.02/
	rm -rf cavalier_matthew.assignment-1.02
final: clean package
analyze: clean debug
	valgrind --log-file="valgrind_output.txt" --leak-check=full ./debug
test: package
	mkdir test
	tar -xf cavalier_matthew.assignment-1.02.tar.gz -C test/

