all: tp_0

tp_0: tp0_main.o
	g++ -o tp_0 tp0_main.o

tp0_main.o: tp0_main.cpp
	g++ -o tp0_main.o -c tp0_main.cpp -O2 -L/usr/X11R6/lib -lm -lpthread -lX11

clean:
	rm -rf *.o

mrproper: clean
	rm -rf tp_0
