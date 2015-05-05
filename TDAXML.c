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

	FILE *archivoxml;

	int error;
	char letra;

	TElem* Elem = (TElem*)malloc(sizeof(TElem));
	TDelimitador* Delimitador = (TDelimitador*)malloc(sizeof(TDelimitador));

	/*Malloc*/
	char* id = malloc (255); /* donde voy guardando las letras */
	char* value = malloc (255);
	*id = 0;
	*value = 0;
	Elem->id = malloc(255);
	Elem->value = malloc(255);
	Delimitador->id = malloc(255);
	Delimitador->estado = (TInterruptor)malloc(sizeof(TInterruptor));

	TDAXml = malloc(sizeof(TDAXML));

	if ((TDAXml->tagPrincipal = (char*)malloc(CANTMAX+1)) == NULL)
		return -1;
	if ((TDAXml->xmlFile = fopen(rutaXml, "r")) == NULL)
		return -2;

	L_Crear(&(TDAXml->atributos),sizeof(TElem));

	/*Malloc*/

	archivoxml = fopen(rutaXml, "r");

	if (archivoxml)
	{

		TDAXml->xmlFile = archivoxml;

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
			/*si empieza con "/" cerrado sino abierto*/
	}

	free(id);
	free(value);
	free(Elem);
	free(Delimitador);

	L_Vaciar(&TDAXml->atributos); /*Esto no tiene que estar acá*/

	if (error == 1)
		error = 0; /* Lista.h está al verrez */

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
	/* Libero la memoria utilizada y cierro el archivo */
	tElem_Destruir(Aux);
	fclose(arch);
	return 0;
}

void TDAXML_Destruir(TDAXML *TDAXml)
{
	free(TDAXml->tagPrincipal);
	L_Vaciar(&(TDAXml->atributos));
}
