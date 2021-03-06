#include <stdio.h>
#include <string.h>
#include "TDAConvertidor.h"

#define MAX_LONG_RUTA 255
#define C_NULO '\0'

#define WRONG_OPE "La operación ingresada es incorrecta.\nDebe ser una de las siguientes:\n-xml2json\n-json2xml\n"
#define OUT_OF_MEM "No existe suficiente memoria.\n"
#define RUTA_ORIGEN "Debe ingresar una ruta de origen a la que se pueda acceder para lectura.\n Debe ubicarse seguido de la operación a realizar.\n"
#define RUTA_DESTINO "Debe ingresar una ruta de destino a la que se pueda acceder para escritura.\n Debe ubicarse seguido de la ruta de origen.\n"
#define LISTA_VACIA "Archivo de entrada vacio.\n"

typedef enum {
	VACIO, /* Aún no se detectó la operación */
	XML2JSON, /* Convertir XML a JSON */
	JSON2XML, /* Convertir JSON a XML */
	ERROR /* Argumento inválido */
} tOperacion;

typedef enum {
	SUCCESS = 1, /* Éxito */
	MEM = -1, /* Error de memoria */
	ORIG = -3, /* Error en la ruta de origen */
	DEST = -2, /* Error en la ruta de destino */
	LISTA = -4 /* Lista Vacia */
} tError;

/* Funcion que se encarga de identificar la operación a realizar
 * @param  arg  argumento ingresado por el usuario
 * @param  orden  identificador de la operación
 * @pos devuelve -1 si el argumento es incorrecto
 */
void Inicializar(char *arg, tOperacion *Ope)
{
	if ((strcmp(arg,"-xml2json") == 0)||(strcmp(arg,"xml2json") == 0))
		*Ope = XML2JSON;
	else
	{
		if ((strcmp(arg,"-json2xml") == 0)||(strcmp(arg,"json2xml") == 0))
			*Ope = JSON2XML;
		else
			*Ope = ERROR;
	}
}

/* Funcion que se encarga de mostrar un mensaje por pantalla de acuerdo al error */
void MostrarMensajeError(tError Code)
{
	switch (Code)
	{
		case(MEM):
		{
			printf(OUT_OF_MEM);
			break;
		}
		case(ORIG):
		{
			printf(RUTA_ORIGEN);
			break;
		}
		case(DEST):
		{
			printf(RUTA_DESTINO);
			break;
		}
		case(LISTA):
		{
			printf(LISTA_VACIA);
			break;
		}
		case(SUCCESS): break;
	}
}

int main(int argc, char *argv[])
{
	TDAConvertidor* TC = CrearTC();
	tOperacion Ope = VACIO;
	tError Code = SUCCESS;
	char* RutaOrigen;
	char* RutaDestino;
	int i = 1;

	if (TC == NULL)
		Code = MEM;

	RutaOrigen = malloc(MAX_LONG_RUTA);
	RutaDestino = malloc(MAX_LONG_RUTA);

	if ((RutaOrigen == NULL) || (RutaDestino == NULL))
		return MEM;

	if (Code == SUCCESS)
	{

		if (argc == 1) /* El usuario no ingresó argumentos, además del nombre del programa */
			printf("Debe ingresar al menos un argumento.\n");
		else
		{
			do
			{

				Ope = VACIO;
				Inicializar(argv[i],&Ope);

				if (Ope != ERROR)
				{
					if (i != (argc-1)) /* No es el último argumento --> puedo obtener la ruta de origen */
					{
						memcpy(RutaOrigen,argv[++i],MAX_LONG_RUTA);

						if (i != (argc-1)) /* No es el último argumento --> puedo obtener la ruta de destino */
							memcpy(RutaDestino,argv[++i],MAX_LONG_RUTA);

						else /* Es el último argumento y no tengo la ruta de destino */
							Code = DEST;
					}
					else /* Es el último argumento y no tengo la ruta de origen */
						Code = ORIG;

					if (Code == SUCCESS)
					{
						switch (Ope)
						{
							case (XML2JSON):
							{
								Code = xml2json(TC,RutaOrigen,RutaDestino);
								break;
							}
							case (JSON2XML):
							{
								Code = json2xml(TC,RutaOrigen,RutaDestino);
								break;
							}
							default: break;
						}
					}
				}
			} while ((++i < argc) && (Code == SUCCESS));
		}
	}

	if (Ope == ERROR)
		printf(WRONG_OPE);

	if (Code != SUCCESS)
		MostrarMensajeError(Code);

	free(RutaOrigen);
	free(RutaDestino);
	DestruirTC(TC);

	return EXIT_SUCCESS;

}
