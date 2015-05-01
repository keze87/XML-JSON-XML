main: main.o
	gcc -ansi -Wall -pedantic -pedantic-errors -o main main.c TDACovertidor.c TDAXML.c TDAJSON.c Lista.c -I. && rm *.o
