all: crawler debug

crawler: crawler.c generator.c
	gcc -Wall -Werror crawler.c generator.c -o crawler
debug: crawler.c generator.c
	gcc -g -Wall -Werror crawler.c generator.c -o debug
run: crawler
	./crawler -cool
clean:
	- rm debug crawler
add: clean
	git add .
exportLog:
	git --no-pager log > CHANGELOG
package:
	mkdir cavalier_matthew.assignment-1.01
	cp generator.* crawler.* Makefile README CHANGELOG cavalier_matthew.assignment-1.01
	tar cvfz cavalier_matthew.assignment-1.01.tar.gz cavalier_matthew.assignment-1.01/
	rm -rf cavalier_matthew.assignment-1.01
