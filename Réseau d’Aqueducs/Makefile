CC = g++ -std=c++17

all: prog 

prog: main.o arbre-couvrant-minimal.hpp tas.hpp arrete.hpp
	$(CC) main.o -o prog
	

main.o: main.cpp
	$(CC) -c main.cpp

cleano: 
	rm -rf *.o

clear:
	rm -rf *.o prog
