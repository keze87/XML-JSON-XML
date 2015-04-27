#include "TDAXML.h"

int xmlCargar(TDAXML *TDAXml, char *rutaXml)
{

	FILE *archivoxml;
	archivoxml = fopen(rutaXml, "r");

	if (archivoxml)
	{

		TDAXml->xmlFile = archivoxml;
		printf("SE ABRIO!\n");



		fclose(archivoxml);
		printf("CARGO XML\n");

	}
	else
		fprintf(stderr,"La ruta %s no es valida\n", rutaXml);

	return 0;

}

int xmlGuardar(TDAXML *TDAXml, char *rutaXml)
{

	FILE *archivoxml;
	archivoxml = fopen(rutaXml, "w");

	if (archivoxml)
	{

		printf("SE ABRIO!\n");



		fclose(archivoxml);
		printf("GUARDO XML\n");

	}
	else
		fprintf(stderr,"La ruta %s no es valida\n", rutaXml);

	return 0;

}
