all: build_static clear

build_static:
	gcc addition.c -c
	gcc subtraction.c -c
	gcc multiplication.c -c
	gcc division.c -c
	
	ar rc libmy_calculater.a addition.o subtraction.o multiplication.o division.o
	gcc main.c -o test -L. -lmy_calculater

build_dynamic:
	gcc functions.c -fPIC -c
	gcc -shared functions.o -o libmy_calculater.so
	
	gcc main.c -o test -L. -lmy_calculater

clear:
	rm -rf addition.o subtraction.o multiplication.o division.o

ex_dyn:
	LD_LIBRARY_PATH=$(PWD) ./test

clear_all:
	rm -rf addition.o subtraction.o multiplication.o division.o functions.o
	rm -rf libmy_calculater.a
	rm -rf libmy_calculater.so
	rm -rf test

