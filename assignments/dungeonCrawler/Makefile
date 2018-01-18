all: crawler debug

crawler: crawler.c
	gcc -Wall -Werror crawler.c -o crawler
debug: crawler.c
	gcc -g crawler.c -o debug
run: crawler
	./crawler
clean:
	rm debug crawler
add: clean
	git add .
