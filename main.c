#include <stdio.h>
#include <string.h>

#include "TDACovertidor.h"

#define XML2JSON 1
#define JSON2XML 2

/*Lectura de argumentos*/
/* Orden = el tipo de funcion que tengamos que usar.
 * posjson,posxml = el valor de argc que da a la ruta de cada uno*/
void inicializar(int argc, char **argv, int *orden, int *posjson, int *posxml,int narg);

int main(int argc, char *argv[])
{

	int orden = 0;
	int posjson = 0;
	int posxml = 0;
	int error = 0;
	int narg = 1; /* Posicion inicio de inicializar */

	TDAConvertidor *tc;

	do
	{

		orden = 0;
		posjson = 0;
		posxml = 0;

		tc = creartc();

		inicializar (argc,argv,&orden,&posjson,&posxml,narg);

		switch (orden)
		{
			case XML2JSON: error = xml2json(tc,argv[posxml],argv[posjson]); narg = narg + 3; break;
			case JSON2XML: error = json2xml(tc,argv[posjson],argv[posxml]); narg = narg + 3; break;
			case 3: error = guardarxml(tc,argv[posxml]); free(tc); return 0;
			default: fprintf(stderr,"Opcion incorrecta.\n"); narg = narg + 1;
		}

		free(tc);

	}while (narg < argc);

	return error;

}

void inicializar(int argc, char **argv, int *orden, int *posjson, int *posxml,int narg)
{

	int auxargc = narg;

	if (strcmp(argv[auxargc],"-xml2json") == 0)
	{
		*orden = XML2JSON;
		*posxml = auxargc + 1;
		*posjson = auxargc + 2;
	}
	else
	{
		if (strcmp(argv[auxargc],"-json2xml") == 0)
		{
			*orden = JSON2XML;
			*posjson = auxargc + 1;
			*posxml = auxargc + 2;
		}
		else
		{
			if (strcmp(argv[auxargc],"-guardarxml") == 0)
			{
				*orden = 3;
				*posxml = auxargc + 1;
			}
		}
	}
}

