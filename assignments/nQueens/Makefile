queen: queens.c proccessing.o
	gcc queens.c -o queen
proccessing.o: proccessing.c
	gcc  -c proccessing.c
queenDebug: queens.c proccessing.c
	gcc -g -o queenDebug queens.c proccessing.c 
testCompile: proccessing.c testQueen.c
	clear
	gcc -g testQueen.c proccessing.c unity.c -o test
test: testCompile
	clear
	./test
clean:
	rm *.o test
