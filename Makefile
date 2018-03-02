CPP=g++
FLAGS=-g -Wall -lGLFW -lglew

Object: Object.cpp Object.h
	$(CPP) $(FLAGS) -c Object.cpp -o Object.o

main: main.cpp Object.o
	$(CPP) $(FLAGS) Object.o main.cpp -o main
