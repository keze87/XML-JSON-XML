#include "TDAXML.h"
#include "Lista.h"
#include "TDAConvertidor.h"

#define CANTMAX 255 /*tamaño maximo de linea*/
#define TAM_ELEM 200
#define DELIM_OPEN "ABIERTO"
#define DELIM_CLOSE "CERRADO"

typedef enum {
	ELEM, /* Es un elemento */
	DELIM, /* Es un delimitador */
	VACIO /* Aún no se definió */
} tTipo;

typedef struct {
	tTipo Tipo;
	char *Atributo;
	char *Value;
} tElem;

/* Inicializa tElem
 * Si no existe suficiente memoria para inicializar la estructura devuelve -1
 */
int tElem_Inicializar(tElem* aux)
{
	if ((aux->Atributo = (char*)malloc(CANTMAX+1)) == NULL)
		return -1;
	if ((aux->Value = (char*)malloc(CANTMAX+1)) == NULL) {
		free(aux->Atributo);
		return -1;
	}
	aux->Tipo=VACIO;

	return 0;
}

/* Libera la memoria utilizada por tElem */
void tElem_Destruir(tElem* aux)
{
	free(aux->Atributo);
	free(aux->Value);
}

/* Funcion que devuelve 0 si el elemento es un delimitador */
int EsDelimitador (tElem aux)
{
	return (aux.Tipo == DELIM)?0:-1;
}

/* Funcion que escribe una cantidad de tabs en el archivo */
void EscribirTabs(int cant, FILE* arch)
{
	while (cant-- > 0)
		fputc('\t', arch);
}
/* Funcion que escribe la apertura de un atributo en un archivo */
void EscribirAtributo_Apertura(char* at, FILE* arch)
{
	fputc('<', arch);
	fputs(at, arch);
	fputc('>', arch);
}

/* Funcion que escribe el cierre de un atributo en un archivo */
void EscribirAtributo_Cierre(char* at, FILE* arch)
{
	fputc('<', arch);
	fputc('/', arch);
	fputs(at, arch);
	fputc('>', arch);
}

int xmlCargar(TDAXML *TDAXml, char *rutaXml)
{

	int error;
	int cont;
	char letra;

	/*Malloc*/
	char* palabra = calloc (255,sizeof(char)); /* donde voy guardando las letras */

	TElem* Elem = (TElem*)malloc(sizeof(TElem));

	Elem->id = calloc(255,sizeof(char));
	Elem->estado = (TInterruptor)malloc(sizeof(TInterruptor));

	TDAXml = malloc(sizeof(TDAXML));

	if ((TDAXml->tagPrincipal = calloc(255,sizeof(char))) == NULL)
		return -1;

	if ((TDAXml->xmlFile = fopen(rutaXml, "r")) == NULL)
	{

		fprintf(stderr,"La ruta %s no es valida\n", rutaXml);

		return -2;

	}

	L_Crear(&(TDAXml->atributos),255 + sizeof(TElem));
	/*Malloc*/

	do
	{

		letra = fgetc(TDAXml->xmlFile);

	}while ((letra != '<') && (letra != EOF));

	while (letra != EOF)
	{

		if (letra == '<')
		{

			memset(palabra,0,255);

			letra = fgetc(TDAXml->xmlFile);

			if (letra == EOF)
				break;

			cont = 0;

			while (letra != '>')
			{

				palabra[cont] = letra;

				letra = fgetc(TDAXml->xmlFile);

				if (letra == EOF)
					break;

				cont++;

			}

			palabra[cont] = '\0';

			strcpy(Elem->id, palabra);

			if(palabra[0] == 47) /* \ */
			{
				Elem->estado = Cerrado;
			}
			else
				Elem->estado = Abierto;

			error = L_Insertar_Cte(&TDAXml->atributos,L_Siguiente,Elem);

		}

		if (letra == '>')
		{

			memset(palabra,0,255);

			letra = fgetc(TDAXml->xmlFile);

			if (letra == EOF)
				break;

			cont = 0;

			while (letra != '<')
			{

				palabra[cont] = letra;

				letra = fgetc(TDAXml->xmlFile);

				if (letra == EOF)
					break;

				cont++;

			}

			palabra[cont] = '\0';

			if ((palabra[0] != '\n') && (palabra[0] != '\t') && (letra == '<'))
			{

				strcpy(Elem->id, palabra);
				Elem->estado = Valor;

				error = L_Insertar_Cte(&TDAXml->atributos,L_Siguiente,Elem);

			}

		}

	}

	error = L_Mover_Cte(&TDAXml->atributos,L_Primero);

	while (error == TRUE)
	{

		L_Elem_Cte(TDAXml->atributos,Elem);

		if (Elem->estado == Valor)
			printf("Valor = %s\n", Elem->id);
		if (Elem->estado == Abierto)
			printf("EL Delim ES: %s (Abierto)\n", Elem->id);
		if (Elem->estado == Cerrado)
			printf("EL Delim ES: %s (Cerrado)\n", Elem->id);

		error = L_Mover_Cte(&TDAXml->atributos,L_Siguiente);

	}

	free(palabra);
	free(Elem);

	if (error == TRUE)
		error = 0;

	return error;

}

int xmlGuardar(TDAXML *TDAXml, char *rutaXml)
{
	tElem* Aux = malloc(sizeof(TElem));
	FILE *arch = fopen(rutaXml,"w");
	int code = 0;
	int nivel = 0; /* Se utiliza para saber cuantos "tabs" imprimir en el archivo */
	if (tElem_Inicializar(Aux) != 0)
		return -1;
	if ((arch = fopen(rutaXml, "w")) == NULL) { /* No se pudo abrir el archivo */
		free(Aux);
		return -2;
	}
	/* Escribo el tagPrincipal */
	EscribirAtributo_Apertura(TDAXml->tagPrincipal,arch);
	fprintf(arch,"\n");
	EscribirTabs(++nivel, arch);
	/* Comienzo a recorrer la lista de atributos */
	if (L_Vacia(TDAXml->atributos)!=0) {
		do {
			code = L_Mover_Cte(&(TDAXml->atributos),L_Primero);
			L_Elem_Cte(TDAXml->atributos,Aux);
			if (EsDelimitador(*Aux)==0) { /* Es un delimitador */
				if (strcmp(Aux->Value,DELIM_OPEN)==0) { /* Es apertura */
					EscribirAtributo_Apertura(Aux->Atributo, arch);
					fprintf(arch, "\n");
					EscribirTabs(++nivel,arch);
				}
				else { /* Es cierre */
					EscribirAtributo_Cierre(Aux->Atributo, arch);
					fprintf(arch, "\n");
					EscribirTabs(--nivel,arch);
				}
			}
			else { /* Es un elemento */
				EscribirAtributo_Apertura(Aux->Atributo, arch);
				fputs(Aux->Value, arch);
				EscribirAtributo_Cierre(Aux->Atributo, arch);
				fprintf(arch, "\n");
				EscribirTabs(nivel,arch);
			}
		} while ((code = L_Mover_Cte(&(TDAXml->atributos),L_Siguiente)) != 0);
	}
	/* Cierro el tagPrincipal */
	EscribirAtributo_Cierre(TDAXml->tagPrincipal,arch);
	/* Libero la memoria utilizada en Aux y cierro el archivo */
	tElem_Destruir(Aux);
	fclose(arch);
	/* Destruyo la estructura TDAXML */
	free(TDAXml->tagPrincipal);
 	L_Vaciar(&TDAXml->atributos);
	return 0;
}
