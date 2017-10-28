# HMIN318_PROJECT
IMAGINA : Projet de fin de semestre de l'unité HMIN318

Le projet consiste à implémenter un(des) algorithme(s) pour la segmentation des Sinus dans des Images CT par seuillage local.

C'est un mini-projet qui rentre dans le cadre de ma formation en imagerie médicale à l'Université Montpellier 2.

Dans ce projet, j'ai utilisé la librairie CImg.h, qui est une librairie c++ pour la manipulation de images 2D et 3D, particulièrement dans la recherche scientifique.

How to compile ?

#g++ : Windows
g++ -o project.exe methods.h main.cpp -O2 -lgdi32

#g++ : Linux version
g++ -o project methods.h main.cpp -O2 -L/usr/X11R6/lib -lm -lpthread -lX11

#g++ : Mac Os
g++ -o project methods.h main.cpp -O2 -lm -lpthread -I/usr/X11R6/include -L/usr/X11R6/lib -lm -lpthread -lX11


Execution example:

./main image_name.hdr
