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

	/*Temporal*/
	FILE *values = fopen("values.DAT","w");
	FILE *ids = fopen("ids.DAT","w");
	/*Temporal*/

	/*char linea[CANTMAX];
	char * delimitadores = "<>";
	char *ret;*/

	char linea; /* una letra */
	char* string = malloc (255); /* donde voy guardando las letras */



	archivoxml = fopen(rutaXml, "r");


	if ((archivoxml) && (values) && (ids))
	{

		/*TDAXml->xmlFile = archivoxml;*/		/*No tiene memoria*/


		/*printf("SE ABRIO!\n");
		fgets(linea, CANTMAX, archivoxml);*/	/*leo la primer linea*/
		/*ret=strtok(linea, delimitadores);
		TDAXml->tagPrincipal=ret;*/			/*obtengo el tag principal*/
		/*printf("TAG PRINCIPAL: %s.\n", TDAXml->tagPrincipal);

		fclose(archivoxml);*/
		printf("CARGO XML\n");

	}
	else
	{

		fprintf(stderr,"La ruta %s no es valida\n", rutaXml);

		return 1;

	}

	do
	{

		linea = fgetc(archivoxml);

	}while ((linea != 60) && (linea != 62) && (linea != EOF));

	while (archivoxml)
	{

		if (linea == 60) /* < */
		{

			linea = fgetc(archivoxml);

			if (linea == EOF)
				break;

			while (linea != 62) /* > */
			{

				fputc(linea,ids);

				linea = fgetc(archivoxml);

				if (linea == EOF)
					break;

			}

			fputc(10,ids); /* \n */

		}

		if (linea == 62) /* > */
		{

			linea = fgetc(archivoxml);

			if (linea == EOF)
				break;

			while (linea != 60) /* < */
			{

				if ((linea != 10/* \n */) && (linea != 9/* TAB */))
					fputc(linea,values);

				linea = fgetc(archivoxml);

				if (linea == EOF)
					break;

			}

			fputc(10,values); /* \n */

		}

		if (linea == EOF)
			break;

	}

	fclose(ids);
	fclose(values);
	free(string);

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
