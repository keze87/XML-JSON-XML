#include "TDAJSON.h"
#include "Lista.h"
#include "TDAConvertidor.h"

#define CANTMAX 255 /*tamaño maximo de linea*/
#define TAM_ELEM 200

int jsonCargar(TDAJSON *TDAJson, char *rutaJson)
{

	TElem Elem;

	char letra;
	char letra_ant;
	char palabra_ant[CANTMAX]; /*la uso por si se repite con el corchete*/

	int i = 0, tope = -1, error = 0;
	int primero_multiple = TRUE; /*flag para saber si es la primera vez que se escribe un atributo multiple*/
	char vector[4][CANTMAX];

	/*inicializo*/
	vector[0][0] = '\0';
	vector[1][0] = '\0';
	vector[2][0] = '\0';
	vector[3][0] = '\0';

	Elem.id[0] = 0; /* palabra */

	if (!TDAJson->jsonFile)
	{

		fprintf(stderr,"La ruta %s no es valida\n", rutaJson);

		return 1;

	}

	printf("SE ABRIO EL ARCHIVO!\n");
	letra = fgetc(TDAJson->jsonFile);

	/*ANTES DE GUARDAR EN LA LISTA, HAY QUE CHEQUEAR SIEMPRE QUE EL ID NO SEA NULO*/

	while(letra != EOF) /*mientras que no se termine el archivo*/
	{

		/*SI ESTA ENTRE COMILLAS*/
		if (letra == 34) /*COMILLA*/ /*recorro hasta que cierre la comilla*/
		{

			letra = fgetc(TDAJson->jsonFile);

			while(letra != 34) /*mientras no cierre comilla, leo y guardo en Elem.id*/
			{
				Elem.id[i] = letra;
				i++;
				letra = fgetc(TDAJson->jsonFile);
			}

			Elem.id[i] = '\0';
			printf("\n%s", Elem.id);

			strcpy(palabra_ant, Elem.id);

			/*obtengo una letra mas y me fijo si es un ":"*/
			letra=fgetc(TDAJson->jsonFile);

			if (letra == ':')
			{
				printf(" -ABRE-");
				tope++;

				strcpy(vector[tope], Elem.id);

				Elem.estado = Abierto;
				if (Elem.id[0] != '\0')
					error = L_Insertar_Cte(&TDAJson->atributos,L_Siguiente,&Elem);
			}

			if ((letra == ',') || (letra == '}'))
			{
				printf(" -VALOR-");
				/*printf("\nTOPE: %d", tope);*/
				printf("\n%s -CIERRA-", vector[tope]);

				Elem.estado = Valor;
				if (Elem.id[0] != '\0')
					error = L_Insertar_Cte(&TDAJson->atributos,L_Siguiente,&Elem);

				strcpy(Elem.id,vector[tope]);
				Elem.estado = Cerrado;
				if (Elem.id[0] != '\0')
					error = L_Insertar_Cte(&TDAJson->atributos,L_Siguiente,&Elem);

				vector[tope][0] = '\0';
				if (tope > 0)
					tope--;
			}

			Elem.id[0] = '\0'; /*limpio la variable*/
			i = 0;

		}

		if (letra == '}')
		{
			printf("\n%s -CIERRA--", vector[tope]);

			strcpy(Elem.id,vector[tope]);
			Elem.estado = Cerrado;
			if (Elem.id[0] != '\0')
				error = L_Insertar_Cte(&TDAJson->atributos,L_Siguiente,&Elem);

			vector[tope][0] = '\0';
			if (tope > 0)
				tope--;
		}

		if (letra == '[')
		{
			vector[tope][0] = '\0'; /*borro el multiple para no repetirlo en el cierre*/
			if (tope > 0)
				tope--;

			while(letra != ']') /*mientras no cierre corchete*/
			{

				if (letra == 34) /*COMILLA*/ /*recorro hasta que cierre la comilla*/
				{

					letra_ant = letra;
					letra = fgetc(TDAJson->jsonFile);

					while (letra != 34) /*mientras no cierre comilla, leo y guardo en Elem.id*/
					{
						Elem.id[i] = letra;
						i++;
						letra_ant = letra;
						letra = fgetc(TDAJson->jsonFile);
					}

					Elem.id[i] = '\0';
					/*vector[tope]=Elem.id;*/

					printf("\n%s", Elem.id);

					letra_ant = letra;
					letra = fgetc(TDAJson->jsonFile); /*obtengo una letra mas y me fijo si es un ":"*/

					if(letra == '}') /*si cierra la llave, tengo que cerrar y volver a abrir el id*/
					{
						printf(" -VALOR-");
						printf("\n%s -CIERRA-", vector[tope]);

						Elem.estado = Valor;
						if (Elem.id[0] != '\0')
							error = L_Insertar_Cte(&TDAJson->atributos,L_Siguiente,&Elem);

						strcpy(Elem.id,vector[tope]);
						Elem.estado = Cerrado;
						if (Elem.id[0] != '\0')
							error = L_Insertar_Cte(&TDAJson->atributos,L_Siguiente,&Elem);

						vector[tope][0] = '\0';
						if (tope > 0)
							tope--;

						printf("\n%s -CIERRA-", palabra_ant);
						strcpy(Elem.id,palabra_ant);
						Elem.estado = Cerrado;
						if (Elem.id[0] != '\0')
							error = L_Insertar_Cte(&TDAJson->atributos,L_Siguiente,&Elem);

						primero_multiple = FALSE;
					}


					if (letra == ':')
					{
						printf(" -ABRE-");
						tope++;
						strcpy(vector[tope], Elem.id);

						Elem.estado = Abierto;
						if (Elem.id[0] != '\0')
							error = L_Insertar_Cte(&TDAJson->atributos,L_Siguiente,&Elem);
					}

					if ((letra == ',') && (letra_ant != '}'))
					{
						printf(" -VALOR-");
						printf("\n%s -CIERRA-", vector[tope]);

						Elem.estado = Valor;
						if (Elem.id[0] != '\0')
							error = L_Insertar_Cte(&TDAJson->atributos,L_Siguiente,&Elem);

						strcpy(Elem.id,vector[tope]);
						Elem.estado = Cerrado;
						if (Elem.id[0] != '\0')
							error = L_Insertar_Cte(&TDAJson->atributos,L_Siguiente,&Elem);

						vector[tope][0] ='\0';
						if (tope > 0)
							tope--;
					}

					Elem.id[0] = '\0'; /*limpio la variable*/
					i = 0;

				}

				if ((letra == '{') && (primero_multiple == FALSE))
				{
					printf("\n%s -ABRE-", palabra_ant);
					strcpy(Elem.id,palabra_ant);
					Elem.estado = Abierto;
					if (Elem.id[0] != '\0')
						error = L_Insertar_Cte(&TDAJson->atributos,L_Siguiente,&Elem);
				}

				letra_ant = letra;
				letra = fgetc(TDAJson->jsonFile);

			}

		}

		letra = fgetc(TDAJson->jsonFile);

	}

	Elem.id[i] = '\0';
	i++;

	fclose(TDAJson->jsonFile);
	printf("\n\nCARGO JSON");

	/* BUSCO EL TAG PRINCIPAL Y LO BORRO DE LA LISTA DE ATRIBUTOS */
	L_Elem_Cte(TDAJson->atributos, &Elem);
	L_Borrar_Cte(&TDAJson->atributos); /*borro el ultimo, que es el cierre del tag principal*/

	error = L_Mover_Cte(&TDAJson->atributos, L_Primero);
	L_Elem_Cte(TDAJson->atributos, &Elem);
	strcpy(TDAJson->tagPrincipal, Elem.id); /*el primer elemento de la lista será el tag principal*/
	L_Borrar_Cte(&TDAJson->atributos); /*borro el primero, que es la apertura del tag principal*/

	if (error != 1)
		return error;

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
