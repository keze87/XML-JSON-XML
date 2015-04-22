#include <stdio.h>
#include <string.h>

#include "TDACovertidor.h"

#define XML2JSON 1
#define JSON2XML 2
/*Lectura de argumentos*/
/* Orden = el tipo de funcion que tengamos que usar. (1->xml2json,2->json2xml)
 * posjson,posxml = el valor de argc que da a la ruta de cada uno, seguro se podria mejorar leyendo la cadena
 * y fijandose que extension tiene*/
void inicializar(int argc, char **argv, int *orden, int *posjson, int *posxml);

int main(int argc, char *argv[])
{

	int orden = 0;
	int posjson = 0;
	int posxml = 0;
	int error = 0;

	TDAConvertidor *tc;

	inicializar (argc,argv,&orden,&posjson,&posxml);

	/*Codigo solo para durante el desarrollo*/
	if (orden != 0)
	{
		printf("\nFuncion: ");
		switch (orden)
		{
			case XML2JSON: printf("xml2json\n"); break;
			case JSON2XML: printf("json2xml\n"); break;
		}
		printf("Ruta XML: %s\n",argv[posxml]);
		printf("Ruta JSON: %s\n",argv[posjson]);
	}
	/*Codigo solo para durante el desarrollo*/

	switch (orden)
	{
		case XML2JSON: error = xml2json(tc,argv[posxml],argv[posjson]); break;
		case JSON2XML: error = json2xml(tc,argv[posjson],argv[posxml]); break;
	}

	return error;

}

void inicializar(int argc, char **argv, int *orden, int *posjson, int *posxml)
{

	int auxargc;

	for (auxargc = 1; auxargc != argc; auxargc++)
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
