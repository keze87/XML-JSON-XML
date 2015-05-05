#include "TDAXML.h"
#include "Lista.h"
#include "TDAConvertidor.h"

#define CANTMAX 255 /*tamaño maximo de linea*/

int TDAXML_Crear(TDAXML *TDAXml, int tamElemento)
{

	TDAXml = malloc(sizeof(TDAXML));

	if ((TDAXml->tagPrincipal = (char*)malloc(CANTMAX+1)) == NULL)
		return -1;

	L_Crear(&(TDAXml->atributos),tamElemento);

	return 0;

}

int xmlCargar(TDAXML *TDAXml, char *rutaXml)
{

	FILE *archivoxml;

	int error;

	TElem* Elem = (TElem*)malloc(sizeof(TElem));
	TDelimitador* Delimitador = (TDelimitador*)malloc(sizeof(TDelimitador));

	/*char linea[CANTMAX];
	char * delimitadores = "<>";
	char *ret;*/

	char letra;

	/*Malloc*/
	char* id = malloc (255); /* donde voy guardando las letras */
	char* value = malloc (255);
	*id = 0;
	*value = 0;
	Elem->id = malloc(255);
	Elem->value = malloc(255);
	Delimitador->id = malloc(255);
	Delimitador->estado = (TInterruptor)malloc(sizeof(TInterruptor));

	/*error = TDAXML_Crear(TDAXml,sizeof(TElem)); No lo puedo hacer andar*/

	TDAXml = malloc(sizeof(TDAXML));
	L_Crear(&(TDAXml->atributos),sizeof(TElem));

	/*Malloc*/

	archivoxml = fopen(rutaXml, "r");

	if (archivoxml)
	{

		TDAXml->xmlFile = archivoxml;

		/*printf("SE ABRIO!\n");
		fgets(linea, CANTMAX, archivoxml);	leo la primer linea
		ret=strtok(linea, delimitadores);
		TDAXml->tagPrincipal=ret;			obtengo el tag principal
		printf("TAG PRINCIPAL: %s.\n", TDAXml->tagPrincipal);
		fclose(archivoxml);

		printf("CARGO XML\n");*/

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

				id[strlen(id)] = letra;

				letra = fgetc(archivoxml);

				if (letra == EOF)
					break;

			}

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

				value[strlen(value)] = letra;

				letra = fgetc(archivoxml);

				if (letra == EOF)
					break;

			}

			if ((value[0] != 10/* \n */) && (value[0] != 9/* TAB */))
				printf("%s\n",value);

		}

		if ((value[0] != 10) && (value[0] != 0))
		{

			strcpy(Delimitador->id,id);
			Delimitador->estado = Abierto;

			error = L_Insertar_Cte(&TDAXml->atributos,L_Siguiente,Delimitador);

			strcpy(Elem->id,id); /* Falta agregar Elem->value, habria que fijarse si está vacio y escribir ahí*/

			error = L_Insertar_Cte(&TDAXml->atributos,L_Siguiente,Elem);

		}
		else
		{

			strcpy(Delimitador->id,id);
			if (id[0] != 47)
				Delimitador->estado = Abierto;
			else
				Delimitador->estado = Cerrado;

			error = L_Insertar_Cte(&TDAXml->atributos,L_Siguiente,Delimitador);

		}
			/*si empieza con "/"" cerrado sino abierto*/
	}

	free(id);
	free(value);
	free(Elem);
	free(Delimitador);

	L_Vaciar(&TDAXml->atributos); /*Esto no tiene que estar acá*/

	return error;

}

int TDAXML_Guardar(TDAXML *TDAXml, char *rutaXml);
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
		return -1;
	return 0;

}

void TDAXML_Destruir(TDAXML *TDAXml)
{
	free(TDAXml->tagPrincipal);
	L_Vaciar(&(TDAXml->atributos));
}
