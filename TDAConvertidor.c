#include "TDAConvertidor.h"
#include "TDAJSON.h"
#include "TDAXML.h"

#define OK 0
#define NO 1

int xml2json(TDAConvertidor *tc, char *rutaXml, char *rutaJson)
{

	int error;

	/*Malloc*/
	tc = malloc(sizeof(TDAXML));

	if ((tc->xml.tagPrincipal = calloc(255,sizeof(char))) == NULL)
		return -1;

	if ((tc->xml.xmlFile = fopen(rutaXml, "r")) == NULL)
	{

		fprintf(stderr,"La ruta %s no es valida\n", rutaXml);

		return -2;

	}

	L_Crear(&(tc->xml.atributos),255 + sizeof(TElem));
	/*Malloc*/

	error = xmlCargar(&tc->xml, rutaXml);

	if (error != OK)
	{
		fprintf(stderr,"%d\n",error);
		return error;
	}

	/*error = jsonGuardar(&tc->json, rutaJson);

	if (error != OK)
	{
		fprintf(stderr,"%d\n",error);
		return error;
	}*/

	error = xmlGuardar(&tc->xml, rutaJson); /*pa  probar*/

	if (error != OK)
	{
		fprintf(stderr,"%d\n",error);
		return error;
	}

	L_Vaciar(&tc->xml.atributos);
	free(tc->xml.tagPrincipal);
	free(tc);

	return error;

}

int json2xml(TDAConvertidor *tc, char *rutaJson, char *rutaXml)
{

	int error;

	error = jsonCargar(&tc->json, rutaJson);

	if (error != OK)
	{
		fprintf(stderr,"%d\n",error);
		return error;
	}

	/*error = Magia*/

	error = xmlGuardar(&tc->xml, rutaXml);

	if (error != OK)
	{
		fprintf(stderr,"%d\n",error);
		return error;
	}

	return error;

}

int CrearTC(TDAConvertidor *tc)
{
	tc = (TDAConvertidor*)malloc(sizeof(TDAConvertidor));
	if (tc == NULL)
		return -1;
	return 0;
}

void DestruirTC(TDAConvertidor *tc)
{
	free(tc);
}
