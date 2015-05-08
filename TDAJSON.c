#include "TDAJSON.h"
#include "Lista.h"
#include "TDAConvertidor.h"

#define CANTMAX 255 /*tamaño maximo de linea*/
#define TAM_ELEM 200

int jsonCargar(TDAJSON *TDAJson, char *rutaJson)
{

	char letra = 0;
	int cont = 0;

	TElem Elem;
	Elem.id = malloc(50);

	while ((letra != '"') && (letra != EOF))
	{

		letra = fgetc(TDAJson->jsonFile);

	}

	while (letra != EOF)
	{

		memset(Elem.id,0,50);

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

			printf("%s\n",Elem.id);

			letra = fgetc(TDAJson->jsonFile);

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

				printf("Value = %s\n",Elem.id);

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
