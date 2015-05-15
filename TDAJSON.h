#ifndef __TDAJSON_H__
#define __TDAJSON_H__

#include "Lista.h"
#include <stdio.h>

typedef struct TDAJSON {
	char *tagPrincipal;
	TListaSimple atributos;
	FILE *jsonFile;
} TDAJSON;

/* @param  TDAXml estrucutra que se inicializará y cargará a partir de rutaJson
 * @param  rutaJson path del archivo xml
 * @pre    -
 * @post   Si se puede cargar la estructura, devuelve 1
 * @post   Si no se puede crear la estructura por falta de memoria, devuelve -1
 * @post   Si no se puede crear la estructura porque no se puede acceder a la ruta para lectura,
 *         devuelve -2
 * @post   Si existe un error de sintaxis en el archivo, devuelve el número de línea
 */
int jsonCargar(TDAJSON *TDAJson, char *rutaJson);

/* @param  TDAJson estructura que se guardará en un archivo
 * @param  rutaJson path de destino
 * @pre    TDAJson creado y cargado
 * @post   Si se puede grabar la estructura en un archivo, devuelve 1
 * @post   Si no existe suficiente memoria, devuelve -1
 * @post   Si no se puede acceder a la ruta para escritura, devuelve -2
 */
int jsonGuardar(TDAJSON *TDAJson, char *rutaJson);

/* @param  TDAJson estructura que se destruirá
 * @pre    TDAJson creado
 * @post   Se destruye la estructura
 */
void TDAJSON_Destruir(TDAJSON *TDAJson);

#endif /* __TDAJSON_H__ */
