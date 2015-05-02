#ifndef __TDAConvertidor_H__
#define __TDAConvertidor_H__

#include "TDAXML.h"
#include "TDAJSON.h"

typedef struct TDAConvertidor{
	TDAXML xml;
	TDAJSON json;
}TDAConvertidor;

typedef struct Nivel1{
	char* id;
	/*char* value;*/
	TListaSimple Level2;
}Nivel1;

typedef struct Nivel2{
	char* id;
	char* value;
	TListaSimple Level3;
}Nivel2;

typedef struct Nivel3{
	char* id;
	char* value;
}Nivel3;

/* PRE: No tiene.
POST: Convierte xml a json según las rutas especificadas.
En caso de error devuelve el mensaje correspondiente. */
int xml2json(TDAConvertidor *tc, char *rutaXml, char *rutaJson);

/* PRE: No tiene.
POST: Convierte json a xml según las rutas especificadas.
En caso de error devuelve el mensaje correspondiente. */
int json2xml(TDAConvertidor *tc, char *rutaJson, char *rutaXml);

TDAConvertidor *creartc();

int guardarxml (TDAConvertidor *tc, char *rutaXml);

#endif /* __TDAConvertidor_H__ */


