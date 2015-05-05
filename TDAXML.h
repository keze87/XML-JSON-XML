#ifndef __TDAXML_H__
#define __TDAXML_H__

#include "Lista.h"
#include <stdio.h>

typedef struct TDAXML {
	char *tagPrincipal;
	TListaSimple atributos;
	FILE *xmlFile;
}TDAXML;

/* @param  TDAXml estructura que se creará
 * @param  tamElemento tamaño del elemento de la lista
 * @pre    -
 * @post   Si se puede crear la estructura, devuelve 0
 * @post   Si no se puede crear la estructura, devuelve -1
 */
int TDAXML_Crear(TDAXML *TDAXml, int tamElemento);

/* @param  TDAXml estrucutra que se cargará
 * @param  rutaXml path del archivo xml
 * @pre    TDAXml creado
 * @post   Si se puede cargar la estructura, devuelve 0
 * @post   Si no se puede acceder a la ruta para lectura, devuelve -1
 * @post   Si existe un error de sintaxis en el archivo, devuelve el número de línea
 */
int xmlCargar(TDAXML *TDAXml, char *rutaXml);

/* @param  TDAXml estructura que se guardará en un archivo
 * @param  rutaXml path de destino
 * @pre    TDAXml creado y cargado
 * @post   Si se puede grabar la estructura en un archivo, devuelve 0
 * @post   Si no se puede acceder a la ruta para escritura, devuelve -1
 */
int TDAXML_Guardar(TDAXML *TDAXml, char *rutaXml);

/* @param  TDAXml estructura que se destruirá
 * @pre    TDAXml creado
 * @post   Se destruye la estructura
 */
void TDAXML_Destruir(TDAXML *TDAXml);

#endif /* __TDAXML_H__ */
