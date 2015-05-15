#ifndef __TDAXML_H__
#define __TDAXML_H__

#include "Lista.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct TDAXML {
	char *tagPrincipal;
	TListaSimple atributos;
	FILE *xmlFile;
}TDAXML;

/* @param  TDAXml estrucutra que se inicializará y cargará a partir de rutaXml
 * @param  rutaXml path del archivo xml
 * @pre    -
 * @post   Si se puede cargar la estructura, devuelve 1
 * @post   Si no se puede crear la estructura por falta de memoria, devuelve -1
 */
int xmlCargar(TDAXML *TDAXml, char *rutaXml);

/* @param  TDAXml estructura que se guardará en un archivo
 * @param  rutaXml path de destino
 * @pre    TDAXml creado y cargado
 * @post   Si se puede grabar la estructura en un archivo, devuelve 1
 * @post   Si no se puede acceder a la ruta para escritura, devuelve -1
 */
int xmlGuardar(TDAXML *TDAXml, char *rutaXml);

#endif /* __TDAXML_H__ */
