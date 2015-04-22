main: main.o
	#gcc -ANSI -c main.c Devuelve:
	#<línea-de-orden>:0:1: error: falta '(' antes del predicado
	gcc -Wall -pedantic -pedantic-errors -o main main.c TDACovertidor.c -I.
