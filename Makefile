CPP=g++
FLAGS=-lGLEW -lglfw -lGL

Object: Object.cpp Object.h
	$(CPP) $(FLAGS) -c Object.cpp -o Object.o

main: main.cpp Object.o
	$(CPP) Object.o main.cpp -o main $(FLAGS) 
