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
	int narg = 1; /* Inicia el for*/

	do
	{

		TDAConvertidor *tc = creartc();

		inicializar (argc,argv,&orden,&posjson,&posxml,narg);

		switch (orden)
		{
			case XML2JSON: error = xml2json(tc,argv[posxml],argv[posjson]); break;
			case JSON2XML: error = json2xml(tc,argv[posjson],argv[posxml]); break;
			default: fprintf(stderr,"Opcion incorrecta.\n");
		}

		free(tc);

		narg = narg + 3;

	}while (narg < argc);

	return error;

}

void inicializar(int argc, char **argv, int *orden, int *posjson, int *posxml,int narg)
{

	int auxargc;

	for (auxargc = narg; auxargc != argc; auxargc++)
	{

		if ((strcmp(argv[auxargc],"-xml2json") == 0) && (*orden == 0))
			*orden = XML2JSON;
		else
		{
			if ((strcmp(argv[auxargc],"-json2xml") == 0) && (*orden == 0))
				*orden = JSON2XML;
			else
			{

				if ((*orden == XML2JSON) && (*posxml == 0))
				{
					*posxml = auxargc;
					*posjson = auxargc + 1;
				}

				if ((*orden == JSON2XML) && (*posjson == 0))
				{
					*posjson = auxargc;
					*posxml = auxargc + 1;
				}

			}
		}
	}
}
