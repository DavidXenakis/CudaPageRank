CC = g++ 
CFLAGS = -std=c++0x -O3
CUDAFLAGS= -DGPU -O3 -lcusparse -o cuda -g -ccbin g++ -m64 -gencode arch=compute_20,code=sm_20 -gencode arch=compute_30,code=sm_30 -gencode arch=compute_35,code=sm_35 -gencode arch=compute_37,code=sm_37 -gencode arch=compute_50,code=sm_50 -gencode arch=compute_52,code=sm_52 -gencode arch=compute_52,code=compute_52 

all: clean main

main: main.o 
	$(CC) $(CFLAGS) -o main *.o 

main.o: 
	$(CC) $(CFLAGS) -c SparseMatrix.cpp Node.cpp Graph.cpp main.cpp

link: main.o 
	nvcc $(CUDAFLAGS) Graph.o main.o Node.o SparseMatrix.o PageRankSparse.cu

test:
	nvcc $(CUDAFLAGS) PageRankSparse.cu

clean:
	rm -rf *.o main

matt: 
	$(CC) Graph.cpp main.cpp Node.cpp PageRank.cpp SparseMatrix.cpp
