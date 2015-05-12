#include "TDAConvertidor.h"
#include "TDAJSON.h"
#include "TDAXML.h"

#define OK 0
#define NO 1

int xml2json(TDAConvertidor *tc, char *rutaXml, char *rutaJson)
{

	int error;

	/*Malloc*/
	tc = malloc(sizeof(TDAConvertidor));

	if ((tc->xml.tagPrincipal = malloc(CANTMAX)) == NULL)
		return -1;

	if ((tc->xml.xmlFile = fopen(rutaXml, "r")) == NULL)
	{
		fprintf(stderr,"La ruta %s no es valida\n", rutaXml);
		return -2;
	}

	L_Crear(&(tc->xml.atributos),sizeof(TElem));
	/*Malloc*/

	error = xmlCargar(&tc->xml, rutaXml);

	if (error != OK)
	{
		fprintf(stderr,"%d\n",error);
		return error;
	}

	/*paso el xml al json*/
	tc->json.atributos = tc->xml.atributos;
	tc->json.tagPrincipal = tc->xml.tagPrincipal;

	error = jsonGuardar(&tc->json, rutaJson);

	if (error != OK)
	{
		fprintf(stderr,"%d\n",error);
		return error;
	}

	/*L_Vaciar(&tc->xml.atributos);*/
	free(tc->xml.tagPrincipal);
	free(tc);

	return error;

}

int json2xml(TDAConvertidor *tc, char *rutaJson, char *rutaXml)
{

	int error;

	/*Malloc*/
	tc = malloc(sizeof(TDAConvertidor));

	if ((tc->json.tagPrincipal = malloc(CANTMAX)) == NULL)
		return -1;

	if ((tc->json.jsonFile = fopen(rutaJson, "r")) == NULL)
	{
		fprintf(stderr,"La ruta %s no es valida\n", rutaJson);
		return -2;
	}

	L_Crear(&(tc->json.atributos),sizeof(TElem));
	/*Malloc*/

	error = jsonCargar(&tc->json, rutaJson);

	if (error != OK)
	{
		fprintf(stderr,"%d\n",error);
		return error;
	}

	/*paso el json al xml*/
	tc->xml.atributos = tc->json.atributos;
	tc->xml.tagPrincipal = tc->json.tagPrincipal;

	error = xmlGuardar(&tc->xml, rutaXml);

	if (error != OK)
	{
		fprintf(stderr,"%d\n",error);
		return error;
	}

	L_Vaciar(&tc->json.atributos);
	free(tc->json.tagPrincipal);
	free(tc);

	return error;

}

int CrearTC(TDAConvertidor *tc)
{
	/*tc = (TDAConvertidor*)malloc(sizeof(TDAConvertidor));
	if (tc == NULL)
		return -1;*/
	return 0;
}

void DestruirTC(TDAConvertidor *tc)
{
	/*free(tc);*/
}
