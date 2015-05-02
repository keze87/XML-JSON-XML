#include "TDAXML.h"
#include "Lista.h"

#define CANTMAX 255 /*tamaño maximo de linea*/

int TDAXML_Crear(TDAXML *TDAXml, int tamElemento)
{
	if ((TDAXml->tagPrincipal = (char*)malloc(CANTMAX+1)) == NULL)
		return -1;
	L_Crear(&(TDAXml->atributos),tamElemento);
	return 0;
}

int xmlCargar(TDAXML *TDAXml, char *rutaXml)
{

	FILE *archivoxml;

	char linea[CANTMAX];
    char * delimitadores = "<>";
    char *ret;

    archivoxml = fopen(rutaXml, "r");


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

void TDAXML_Destruir(TDAXML *TDAXml)
{
	free(TDAXml->tagPrincipal);
	L_Vaciar(&(TDAXml->atributos));
}
