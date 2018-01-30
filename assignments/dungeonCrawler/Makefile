all: crawler debug
crawler: crawler.o generator.o list.o
	gcc -Wall -Werror crawler.o generator.o list.o -o crawler
debug: crawler.o generator.o list.o
	gcc -g -Wall -Werror crawler.o generator.o list.o -o debug
crawler.o: crawler.c
	gcc -g -Wall -Werror -c crawler.c
generator.o: generator.c
	gcc -g -Wall -Werror -c generator.c
list.o: list.c
	gcc -g -Wall -Werror -c list.c
run: crawler
	./crawler -cool --save --load 
clean:
	- rm debug crawler
	- rm *.o
add: clean
	git add .
exportLog:
	git --no-pager log > CHANGELOG
package:
	mkdir cavalier_matthew.assignment-1.02
	cp generator.* crawler.* Makefile README CHANGELOG cavalier_matthew.assignment-1.02
	tar cvfz cavalier_matthew.assignment-1.02.tar.gz cavalier_matthew.assignment-1.02/
	rm -rf cavalier_matthew.assignment-1.02
