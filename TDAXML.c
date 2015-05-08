#include "TDAXML.h"
#include "Lista.h"
#include "TDAConvertidor.h"

#define CANTMAX 255 /*tamaño maximo de linea*/

/* Funcion que escribe una cantidad de tabs en el archivo */
void EscribirTabs(int cant, FILE* arch)
{
	while (cant-- > 0)
		fputc('\t', arch);
}
/* Funcion que escribe la apertura de un atributo en un archivo */
void EscribirAtributo(char* at, FILE* arch)
{
	fputc('<', arch);
	fputs(at, arch);
	fputc('>', arch);
}

int xmlCargar(TDAXML *TDAXml, char *rutaXml)
{

	int error;
	int cont;
	char letra;

	int tag = 0; /* para el tagpcpal */

	/*Malloc*/
	TElem Elem;
	Elem.id = malloc(255); /* donde voy guardando las letras */
	/*Malloc*/

	do
	{

		letra = fgetc(TDAXml->xmlFile);

	}while ((letra != '<') && (letra != EOF));

	while (letra != EOF)
	{

		free(Elem.id);
		Elem.id = calloc(50,sizeof(char));

		if (letra == '<')
		{

			letra = fgetc(TDAXml->xmlFile);

			if (letra == EOF)
				break;

			cont = 0;

			while (letra != '>')
			{

				Elem.id[cont] = letra;

				letra = fgetc(TDAXml->xmlFile);

				if (letra == EOF)
					break;

				cont++;

			}

			if (tag != 0)
			{

				if(Elem.id[0] == '/')
				{
					Elem.estado = Cerrado;
				}
				else
					Elem.estado = Abierto;

				error = L_Insertar_Cte(&TDAXml->atributos,L_Siguiente,&Elem);

				printf("%s\n",Elem.id);

			}
			else /* Solo la primera vez */
			{

				strcpy(TDAXml->tagPrincipal,Elem.id);
				printf("tagPrincipal = %s\n\n",TDAXml->tagPrincipal);
				tag = 1;

			}

		}
		else
		{

			if (letra == '>')
			{

				letra = fgetc(TDAXml->xmlFile);

				if (letra == EOF)
					break;

				cont = 0;

				while (letra != '<')
				{

					Elem.id[cont] = letra;

					letra = fgetc(TDAXml->xmlFile);

					if (letra == EOF)
						break;

					cont++;

				}

				Elem.id[cont] = '\0';

				if ((Elem.id[0] != '\n') && (Elem.id[0] != '\t') && (letra == '<'))
				{

					Elem.estado = Valor;

					error = L_Insertar_Cte(&TDAXml->atributos,L_Siguiente,&Elem);

					printf("\nvalor = %s\n\n",Elem.id);

				}

			}

		}

	}

	if (error == TRUE)
		error = 0;

	return error;

}

int xmlGuardar(TDAXML *TDAXml, char *rutaXml)
{

	TElem Aux;
	FILE *arch = fopen(rutaXml,"w");
	int code = 0;
	int nivel = 0; /* Se utiliza para saber cuantos "tabs" imprimir en el archivo */

	if (arch == NULL)
	{
		fprintf(stderr,"La ruta %s no es valida\n", rutaXml);
		return -1;
	}

	/* Escribo el tagPrincipal */
	EscribirAtributo(TDAXml->tagPrincipal,arch);
	fprintf(arch,"\n");
	EscribirTabs(++nivel, arch);

	/* Comienzo a recorrer la lista de atributos */
	code = L_Mover_Cte(&(TDAXml->atributos),L_Primero);

	while (code != FALSE)
	{

		L_Elem_Cte(TDAXml->atributos,&Aux);

		if (Aux.estado == Valor) /* Es un elemento */
		{
			fputs(Aux.id, arch);
			/*fprintf(arch,"\n");*/
		}
		else
		{

			EscribirAtributo(Aux.id,arch);

			if (Aux.estado == Abierto)
				EscribirTabs(++nivel,arch);
			else
			{
				fputc('\n', arch);
				EscribirTabs(--nivel,arch);
			}

		}

		code = L_Mover_Cte(&(TDAXml->atributos),L_Siguiente);

	}

	/* Cierro el archivo */
	fclose(arch);

	return 0;

}
