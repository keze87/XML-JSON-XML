main: main.o
	gcc -ansi -Wall -pedantic -pedantic-errors -o main main.c TDAConvertidor.c TDAXML.c TDAJSON.c Lista.c -I.
