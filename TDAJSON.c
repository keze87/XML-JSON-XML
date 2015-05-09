#include "TDAJSON.h"
#include "Lista.h"
#include "TDAConvertidor.h"

#define CANTMAX 255 /*tamaño maximo de linea*/
#define TAM_ELEM 200

int jsonCargar(TDAJSON *TDAJson, char *rutaJson)
{

	char letra = 0;
	int cont = 0;
	int tag = 0;

	TElem Elem;

	while ((letra != '"') && (letra != EOF))
	{

		letra = fgetc(TDAJson->jsonFile);

	}

	while (letra != EOF)
	{

		cont = 0;

		while (cont != 51)
		{
			Elem.id[cont] = 0;
			cont++;
		}

		if (letra == '"')
		{

			letra = fgetc(TDAJson->jsonFile);

			cont = 0;

			while (letra != '"')
			{

				Elem.id[cont] = letra;

				letra = fgetc(TDAJson->jsonFile);

				cont++;

			}

			letra = fgetc(TDAJson->jsonFile);

			if (tag == 0)
			{

				strcpy(TDAJson->tagPrincipal,Elem.id);

				tag = 1;

				printf("Tag Pcpal: %s\n",Elem.id);

			}
			else
			{

				printf("%s\n",Elem.id);

			}

		}
		else
		{

			if (letra == ':')
			{

				letra = fgetc(TDAJson->jsonFile);

				if (letra == '"')
				{

					letra = fgetc(TDAJson->jsonFile);

					cont = 0;

					while (letra != '"')
					{

						Elem.id[cont] = letra;

						letra = fgetc(TDAJson->jsonFile);

						cont++;

					}

					printf("Valor = %s\n",Elem.id);

					letra = fgetc(TDAJson->jsonFile);

				}
			}
			else
				letra = fgetc(TDAJson->jsonFile);

		}

	}

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
