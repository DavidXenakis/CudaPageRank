CC = g++
CFLAGS = -std=c++0x 
FLAGS= -DGPU -lcusparse -o cuda -g -ccbin g++ -m64 -gencode arch=compute_20,code=sm_20 -gencode arch=compute_30,code=sm_30 -gencode arch=compute_35,code=sm_35 -gencode arch=compute_37,code=sm_37 -gencode arch=compute_50,code=sm_50 -gencode arch=compute_52,code=sm_52 -gencode arch=compute_52,code=compute_52 

all: clean main

main: main.o 
	$(CC) $(CFLAGS) -o main *.o 


main.o: 
	$(CC) $(CFLAGS) -c *.cpp

test:
	nvcc $(FLAGS) PageRankSparse.cpp -DGPU


clean:
	rm -rf *.o main 
