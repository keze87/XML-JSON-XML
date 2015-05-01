main: main.o
	gcc -ansi -Wall -pedantic -pedantic-errors -o main main.c TDACovertidor.c TDAXML.c TDAJSON.c -I. && rm *.o
