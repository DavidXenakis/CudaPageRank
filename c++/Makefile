CC = g++
CFLAGS = -std=c++0x 


all: clean main

main: main.o 
	$(CC) $(CFLAGS) -o main *.o 


main.o: 
	$(CC) $(CFLAGS) -c *.cpp


clean:
	rm -rf *.o main 