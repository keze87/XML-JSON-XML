main: main.o TDACovertidor.o
	gcc -o main main.c TDACovertidor.c -I.
