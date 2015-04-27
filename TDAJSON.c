#include "TDAJSON.h"

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
