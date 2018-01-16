all: crawler debug

crawler: crawler.c
	gcc crawler.c -o crawler
debug: crawler.c
	gcc -g crawler.c -o debug
clean:
	rm debug crawler
