CPP=g++
FLAGS=-lGLEW -lglfw -lGL

Object: Object.cpp Object.h
	$(CPP) $(FLAGS) -c Object.cpp -o Object.o

main: main.cpp Object.o
	$(CPP) Object.o main.cpp -o main $(FLAGS)

glad: glad/glad.h glad.c
	$(CPP) -c glad.c -o glad.o

shader: shader.h
	$(CPP) -c shader.h -o shader.o glad.o

mesh: mesh.h
	$(CPP) shader.o glad.o -c mesh.h -o mesh.o

model: model.h stb_image.h
	$(CPP) -lassimp mesh.o shader.o glad.o -c model.h -o model.o
