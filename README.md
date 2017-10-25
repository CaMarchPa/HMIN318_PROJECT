# HMIN318_PROJECT
IMAGINA : Projet de fin de semestre de l'unité HMIN318

Le projet consiste à implémenter un algorithme de segmentation des Sinus à partir d'Images CT.

C'est un petit projet qui rentre dans le cadre de ma formation en imagerie médicale.

Dans ce projet, j'ai utilisé la librairie CImg.h, qui est un librairie cpp pour la manipulation de images 2D et 3D.

How to compile ?

#g++ : Windows
g++ -o project.exe methods.h main.cpp -O2 -lgdi32

#g++ : Linux version
g++ -o project methods.h main.cpp -O2 -L/usr/X11R6/lib -lm -lpthread -lX11

#g++ : Mac Os
g++ -o project methods.h main.cpp -O2 -lm -lpthread -I/usr/X11R6/include -L/usr/X11R6/lib -lm -lpthread -lX11


Execution example:

./main image_name.hdr
