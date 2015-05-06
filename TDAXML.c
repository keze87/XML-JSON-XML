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
	int cont;
	char letra;

	TElem* Elem = (TElem*)malloc(sizeof(TElem));

	/*Malloc*/
	char* id = malloc (255); /* donde voy guardando las letras */
	char* value = malloc (255);
	*id = 0;
	*value = 0;
	Elem->id = malloc(255);
	Elem->estado = (TInterruptor)malloc(sizeof(TInterruptor));

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

	}while ((letra != 60) && (letra != EOF));

	while (letra != EOF)
	{

		if(L_Vacia((TDAXml->atributos))==FALSE){
			L_Elem_Cte((TDAXml->atributos), Elem);
			if (Elem->estado == Valor)
				printf("EL ELEM ES: %s\n", Elem->id);
			else
				printf("EL Delim ES: %s %d\n", Elem->id, Elem->estado);
		}



		if (letra == 60) /* < */
		{

			memset(id,0,255);

			letra = fgetc(archivoxml);

			if (letra == EOF)
				break;

			cont = 0;

			while (letra != 62) /* > */
			{

				id[cont] = letra;

				printf("%c",letra);

				letra = fgetc(archivoxml);

				if (letra == EOF)
					break;

				cont++;

			}

			printf("\n");

			strcpy(Elem->id, id);
			if(id[0] == 47)
			{
				Elem->estado = Cerrado;
			}
			else
				Elem->estado = Abierto;

			error = L_Insertar_Cte(&TDAXml->atributos,L_Siguiente,Elem);
		}

		if (letra == 62) /* > */
		{

			memset(value,0,255);

			letra = fgetc(archivoxml);

			if (letra == EOF)
				break;

			cont = 0;

			while (letra != 60) /* < */
			{

				value[/*strlen(value)*/cont] = letra;

				letra = fgetc(archivoxml);

				if (letra == EOF)
					break;

				cont++;

			}

			if ((value[0] != 10/* \n */) && (value[0] != 9/* TAB */)){
				/*printf("value = %s\n",value);*/

				strcpy(Elem->id, value);
				Elem->estado = Valor;

				error = L_Insertar_Cte(&TDAXml->atributos,L_Siguiente,Elem);

			}
			else
				letra = 60;

		}

	}

	free(id);
	free(value);
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
