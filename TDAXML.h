#ifndef __TDAXML_H__
#define __TDAXML_H__

#include "Lista.h"
#include <stdio.h>

typedef struct TDAXML {
	char *tagPrincipal;
	TListaSimple atributos;
	FILE *xmlFile;
}TDAXML;

/* PRE: No tiene
POST: Inicializa el TDAXML con el archivo xml que persiste en rutaXml. */
int xmlCargar(TDAXML *TDAXml, char *rutaXml);

/* PRE: TDAXml correctamente inicializado en memoria
POST: Guarda el TDAXML en el archivo xml que persistirá en rutaXml. */
int xmlGuardar(TDAXML *TDAXml, char *rutaXml);

#endif /* __TDAXML_H__ */
