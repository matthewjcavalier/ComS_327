all: queens debug
run: queens
	./queens > output.txt
queens: queens.c
	gcc queens.c -o queens
debug: queens.c
	gcc -g queens.c -o debug
clean:
	rm queens debug