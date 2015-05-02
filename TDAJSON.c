#include "TDAJSON.h"
#include "Lista.h"

#define CANTMAX 255 /*tamaño maximo de linea*/

int TDAJSON_Crear(TDAJSON *TDAJson, int tamElemento)
{
	if ((TDAJson->tagPrincipal = (char*)malloc(CANTMAX+1)) == NULL)
		return -1;
	L_Crear(&(TDAJson->atributos),tamElemento);
	return 0;
}

int jsonCargar(TDAJSON *TDAJson, char *rutaJson)
{

	FILE *archivojson;
	archivojson = fopen(rutaJson, "r");

	if (archivojson)
	{

		TDAJson->jsonFile = archivojson;
		printf("SE ABRIO!\n");



		fclose(archivojson);
		printf("CARGO JSON\n");

	}
	else
		fprintf(stderr,"La ruta %s no es valida\n", rutaJson);

	return 0;

}

int jsonGuardar(TDAJSON *TDAJson, char *rutaJson)
{

	FILE *archivojson;
	archivojson = fopen(rutaJson, "w");

	if (archivojson)
	{

		printf("SE ABRIO!\n");



		fclose(archivojson);
		printf("GUARDO JSON\n");

	}
	else
		fprintf(stderr,"La ruta %s no es valida\n", rutaJson);

	return 0;

}

void TDAJSON_Destruir(TDAJSON *TDAJson)
{
	free(TDAJson->tagPrincipal);
	L_Vaciar(&(TDAJson->atributos));
}
