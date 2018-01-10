queen: queens.c
	gcc queens.c -o queen
proccessing.o: proccessing.c
	gcc -g -c proccessing.c
testCompile: proccessing.c testQueen.c
	gcc -g testQueen.c proccessing.c unity.c -o test
test: testCompile
	clear
	./test
clean:
	rm *.o test
