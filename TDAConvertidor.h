#ifndef __TDAConvertidor_H__
#define __TDAConvertidor_H__

#include "TDAXML.h"
#include "TDAJSON.h"

#define CANTMAX 2901

typedef enum TInterruptor{
	Abierto = 0,
	Cerrado = 1,
	Valor = 2
}TInterruptor;

typedef struct TDAConvertidor{
	TDAXML xml;
	TDAJSON json;
}TDAConvertidor;

typedef struct TElem{
	char id[CANTMAX];
	TInterruptor estado;
}TElem;


/* @param  rutaXml   ruta dónde se encuentra el archivo XML
 * @param  rutaJson  ruta dónde se grabará el archivo JSON
 * @pre    -
 * @post   Si se puede realizar la conversión y guardar el nuevo archivo en la ruta especificada, devuelve 1
 * @post   Si no existe suficiente memoria, devuelve -1
 * @post   Si no se puede acceder a rutaXml para lectura, devuelve -2
 * @post   Si no se puede acceder a rutaJson para escritura, devuelve -3
 */
int xml2json(TDAConvertidor *tc, char *rutaXml, char *rutaJson);

/* @param  rutaJson  ruta dónde se se encuentra el archivo JSON
 * @param  rutaXml   ruta dónde se grabará el archivo XML
 * @pre    -
 * @post   Si se puede realizar la conversión y guardar el nuevo archivo en la ruta especificada, devuelve 1
 * @post   Si no existe suficiente memoria, devuelve -1
 * @post   Si no se puede acceder a rutaJson para lectura, devuelve -2
 * @post   Si no se puede acceder a rutaXml para escritura, devuelve -3
 */
int json2xml(TDAConvertidor *tc, char *rutaJson, char *rutaXml);

/* PRE:  No tiene
 * POST: Asigna memoria a la estructura
 *       En caso de error devuelve NULL
 */
TDAConvertidor *CrearTC();

/* @pre    tc creado
 * @post   Libera la memoria asignada a la estructura
 */
void DestruirTC(TDAConvertidor *tc);

/* @pre    Listas Destino y Origen creadas
 * @post   Los Elementos de Origen quedan en Destino
 */
int CopiarLista (TListaSimple *Destino, TListaSimple *Origen);

#endif /* __TDAConvertidor_H__ */


