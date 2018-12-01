CC=g++ -g
LIBS=-lsfml-graphics -lsfml-window -lsfml-system

all:
	$(CC) *.cpp -o fs.out $(LIBS)
