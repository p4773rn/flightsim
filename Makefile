CC=g++
LIBS=-lsfml-graphics -lsfml-window -lsfml-system


all: main.cpp
	$(CC) -o fc.out main.cpp $(LIBS)