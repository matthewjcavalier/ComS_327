all: queens debug
run: queens
	./queens > output.txt
queens: queens.c
	gcc queens.c -o queens
debug: queens.c
	gcc -g queens.c -o debug
clean:
	rm queens debug output.txt
exportLog:
	git --no-pager log > log.txt
package:
	mkdir cavalier_matthew.assignment-0
	cp queens.c Makefile CHANGELOG README cavalier_matthew.assignment-0
	tar cvfz cavalier_matthew.assignment-0.tar.gz cavalier_matthew.assignment-0/
	rm -rf cavalier_matthew.assignment-0