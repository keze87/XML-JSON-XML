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

	char letra;

	char* id = malloc (255); /* donde voy guardando las letras */
	char* value = malloc (255);
	*id = 0;
	*value = 0;

	archivoxml = fopen(rutaXml, "r");

	if ((archivoxml) && (values) && (ids))
	{

		/*TDAXml->xmlFile = archivoxml;*/		/*No tiene memoria*/

		/*printf("SE ABRIO!\n");
		fgets(linea, CANTMAX, archivoxml);	leo la primer linea
		ret=strtok(linea, delimitadores);
		TDAXml->tagPrincipal=ret;			obtengo el tag principal
		printf("TAG PRINCIPAL: %s.\n", TDAXml->tagPrincipal);
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

		letra = fgetc(archivoxml);

	}while ((letra != 60) && (letra != 62) && (letra != EOF));

	while (letra != EOF)
	{

		if (letra == 60) /* < */
		{

			memset(id,0,strlen(id));
			memset(value,0,strlen(value));

			letra = fgetc(archivoxml);

			if (letra == EOF)
				break;

			while (letra != 62) /* > */
			{

				fputc(letra,ids);

				id[strlen(id)] = letra;

				letra = fgetc(archivoxml);

				if (letra == EOF)
					break;

			}

			fputc(10,ids); /* \n */
			printf("%s\n",id);

		}

		if (letra == 62) /* > */
		{

			memset(id,0,strlen(id));
			memset(value,0,strlen(value));

			letra = fgetc(archivoxml);

			if (letra == EOF)
				break;

			while (letra != 60) /* < */
			{

				id[strlen(id)] = letra;

				if ((letra != 10/* \n */) && (letra != 9/* TAB */))
					fputc(letra,values);

				letra = fgetc(archivoxml);

				if (letra == EOF)
					break;

			}

			fputc(10,values); /* \n */
			printf("%s\n",id);

		}

	}

	fclose(ids);
	fclose(values);
	free(id);
	free(value);

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
