typedef struct TDAConvertidor{
  TDAXML xml;
  TDAJSON json;
}TDAConvertidor;

/* PRE: No tiene.
POST: Convierte xml a json según las rutas especificadas.
En caso de error devuelve el mensaje correspondiente. */
int xml2json(TDAConvertidor * tc, char * rutaXml, char * rutaJson);

/* PRE: No tiene.
POST: Convierte json a xml según las rutas especificadas.
En caso de error devuelve el mensaje correspondiente. */
int json2xml(TDAConvertidor * tc, char * rutaJson, char * rutaXml);
