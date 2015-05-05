#include "TDAConvertidor.h"
#include "TDAJSON.h"
#include "TDAXML.h"

#define OK 0
#define NO 1

int xml2json(TDAConvertidor *tc, char *rutaXml, char *rutaJson)
{

	int error;

	error = xmlCargar(&tc->xml, rutaXml);

	if (error != OK)
	{
		fprintf(stderr,"%d\n",error);
		return error;
	}

	/*error = Magia*/

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

	error = jsonCargar(&tc->json, rutaJson);

	if (error != OK)
	{
		fprintf(stderr,"%d\n",error);
		return error;
	}

	/*error = Magia*/

	/*error = xmlGuardar(&tc->xml, rutaXml);*/

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
