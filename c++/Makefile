CC = g++
CFLAGS = -std=c++0x 


all: clean main

main: main.o 
	$(CC) $(CFLAGS) -o main *.o 


main.o: 
	$(CC) $(CFLAGS) -c Graph.cpp main.cpp Node.cpp PageRank.cpp SparseMatrix.cpp


clean:
	rm -rf *.o main

matt: 
	$(CC) Graph.cpp main.cpp Node.cpp PageRank.cpp SparseMatrix.cpp
