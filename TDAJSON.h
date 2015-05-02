#ifndef __TDAJSON_H__
#define __TDAJSON_H__

#include "Lista.h"
#include <stdio.h>

typedef struct TDAJSON {
	char *tagPrincipal;
	TListaSimple atributos;
	FILE *jsonFile;
} TDAJSON;

/* @param  TDAJSON estructura que se creará
 * @param  tamElemento tamaño del elemento de la lista
 * @pre    -
 * @post   Si se puede crear la estructura, devuelve 0
 * @post   Si no se puede crear la estructura, devuelve -1
 */
int TDAJSON_Crear(TDAJSON *TDAJSON, int tamElemento);

/* @param  TDAJson estrucutra que se cargará
 * @param  rutaJson path del archivo xml
 * @pre    TDAJson creado
 * @post   Si se puede cargar la estructura, devuelve 0
 * @post   Si no se puede acceder a la ruta para lectura, devuelve -1
 * @post   Si existe un error de sintaxis en el archivo, devuelve el número de línea
 */
int jsonCargar(TDAJSON *TDAJson, char *rutaJson);

/* PRE: TDAJSON correctamente inicializado en memoria
POST: Guarda el TDAJSON en el archivo json que persistirá en rutaJson. */
int jsonGuardar(TDAJSON *TDAJson, char *rutaJson);

#endif /* __TDAJSON_H__ */
