all: build clear
build:
	gcc addition.c -c
	gcc subtraction.c -c
	gcc multiplication.c -c
	gcc division.c -c
	ar rc libmy_calculater.a addition.o subtraction.o multiplication.o division.o
	gcc main.c -o test -L. -lmy_calculater
clear:
	rm -rf addition.o subtraction.o multiplication.o division.o

del_lib:
	rm libmy_calculater.a
del_exec:
	rm test
