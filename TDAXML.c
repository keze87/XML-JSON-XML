#include "TDAXML.h"
#define CANTMAX 255 /*tamaño maximo de linea*/

int xmlCargar(TDAXML *TDAXml, char *rutaXml)
{

	FILE *archivoxml;
	archivoxml = fopen(rutaXml, "r");

	char linea[CANTMAX];
    char * delimitadores = "<>";
    char *ret;

	if (archivoxml)
	{

		TDAXml->xmlFile = archivoxml;
		printf("SE ABRIO!\n");
        fgets(linea, CANTMAX, archivoxml);  /*leo la primer linea*/
        ret=strtok(linea, delimitadores);
        TDAXml->tagPrincipal=ret;           /*obtengo el tag principal*/
        printf("TAG PRINCIPAL: %s.\n", TDAXml->tagPrincipal);

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
