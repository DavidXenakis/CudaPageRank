CC = g++ 
CFLAGS = -std=c++0x -O3 -g
CUDAFLAGS= -DGPU -O3 -lcusparse -o pageRankCuda -g -ccbin g++ -m64 -gencode arch=compute_20,code=sm_20 -gencode arch=compute_30,code=sm_30 -gencode arch=compute_35,code=sm_35 -gencode arch=compute_37,code=sm_37 -gencode arch=compute_50,code=sm_50 -gencode arch=compute_52,code=sm_52 -gencode arch=compute_52,code=compute_52 
MICFLAGS= -g -debug inline-debug-info -O3 -o pageRankMic -openmp -mkl -Llibmkl_scalapack_lp64 -qopt-report=3 
MKLLIB= -L/opt/intel/mkl/lib/mic/libmkl_intel_lp64.so -L/opt/intel/mkl/lib/mic/libmkl_scalapack_lp64.so

all: clean main cuda

main: main.o 
	$(CC) $(CFLAGS) -o main *.o 

main.o: 
	$(CC) $(CFLAGS) -c SparseMatrix.cpp Node.cpp Graph.cpp main.cpp

cuda: main.o 
	nvcc $(CUDAFLAGS) Graph.o main.o Node.o SparseMatrix.o PageRankSparse.cu

mic: main.o
	icc Graph.o main.o Node.o SparseMatrix.o PageRankSparse.cpp $(MICFLAGS)

test:
	nvcc $(CUDAFLAGS) PageRankSparse.cu

clean:
	rm -rf *.o main

matt: 
	$(CC) Graph.cpp main.cpp Node.cpp PageRank.cpp SparseMatrix.cpp
