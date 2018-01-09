queen: queens.c
	gcc queens.c -o queen
proccessing.o: proccessing.c
	gcc -c proccessing.c
test: proccessing.o testQueen.c
	gcc testQueen.c proccessing.o unity.c -o test
	clear
	./test
	
clean:
	rm *.o test
