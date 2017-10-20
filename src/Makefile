all: project

project: main.o
	g++ -o project main.o -O2 -L/usr/X11R6/lib -lm -lpthread -lX11

main.o: methods.h main.cpp
	g++ -o main.o main.cpp -O2 -L/usr/X11R6/lib -lm -lpthread -lX11

clean:
	rm -rf *.o

mrproper: clean
	rm -rf project
