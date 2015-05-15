#include "TDAConvertidor.h"
#include "TDAJSON.h"
#include "TDAXML.h"

#define OK 1

int xml2json(TDAConvertidor *tc, char *rutaXml, char *rutaJson)
{

	int error;

	/*Malloc*/
	if ((tc->xml.xmlFile = fopen(rutaXml, "r")) == NULL)
	{
		fprintf(stderr,"La ruta %s no es valida\n", rutaXml);
		return -3;
	}
	/*Malloc*/

	error = xmlCargar(&tc->xml, rutaXml);

	if (error != OK)
	{
		fprintf(stderr,"%d\n",error);
		return error;
	}

	error = jsonGuardar(&tc->json, rutaJson);

	if (error != OK)
	{
		fprintf(stderr,"%d\n",error);
		return error;
	}

	return error;

}

int json2xml(TDAConvertidor *tc, char *rutaJson, char *rutaXml)
{

	int error;

	/*Malloc*/
	if ((tc->json.jsonFile = fopen(rutaJson, "r")) == NULL)
	{
		fprintf(stderr,"La ruta %s no es valida\n", rutaJson);
		return -3;
	}
	/*Malloc*/

	error = jsonCargar(&tc->json, rutaJson);

	if (error != OK)
	{
		fprintf(stderr,"%d\n",error);
		return error;
	}

	if (L_Vacia (tc->json.atributos) == TRUE)
		return error;

	error = xmlGuardar(&tc->xml, rutaXml);

	if (error != OK)
	{
		fprintf(stderr,"%d\n",error);
		return error;
	}

	return error;

}

TDAConvertidor *CrearTC()
{
	TDAConvertidor *tc;

	if ((tc = (TDAConvertidor*)malloc(sizeof(TDAConvertidor))) == NULL)
		return NULL;

	if ((tc->xml.tagPrincipal = (char*)malloc(CANTMAX)) == NULL)
	{
		free(tc);

		return NULL;
	}

	if ((tc->json.tagPrincipal = (char*)malloc(CANTMAX)) == NULL)
	{
		free(tc->xml.tagPrincipal);
		free(tc);
		return NULL;
	}

	L_Crear(&(tc->xml.atributos),sizeof(TElem));

	L_Crear(&(tc->json.atributos),sizeof(TElem));

	return tc;
}

void DestruirTC(TDAConvertidor *tc)
{
	L_Vaciar(&tc->xml.atributos);
	L_Vaciar(&tc->json.atributos);

	free(tc->xml.tagPrincipal);
	free(tc->json.tagPrincipal);

	free(tc);
}
