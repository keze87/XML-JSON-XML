#ifndef __TDAJSON_H__
#define __TDAJSON_H__
#include <stdio.h>
#include "Lista.h"

typedef struct TDAJSON {
	char *tagPrincipal;
	TListaSimple atributos;
	FILE *jsonFile;
} TDAJSON;

/* PRE: No tiene
POST: Inicializa el TDAJSON con el archivo xml que persiste en rutaJson. */
int jsonCargar(TDAJSON *TDAJson, char *rutaJson);

/* PRE: TDAJSON correctamente inicializado en memoria
POST: Guarda el TDAJSON en el archivo json que persistirá en rutaJson. */
int jsonGuardar(TDAJSON *TDAJson, char *rutaJson);

#endif /* __TDAJSON_H__ */
