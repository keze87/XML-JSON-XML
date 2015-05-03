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

	/*error = jsonGuardar(&tc->json, rutaJson);

	if (error != OK)
	{
		fprintf(stderr,"%d\n",error);
		return error;
	}*/

	fclose(tc->xml.xmlFile);

	return error;

}

int guardarxml (TDAConvertidor *tc, char *rutaXml)
{

	FILE* arch = fopen(rutaXml,"w");

	int error;

	/** Definicion de estructura */

	Nivel1 *e1 = (Nivel1*)malloc(sizeof(Nivel1));
		Nivel2 *e2 = (Nivel2*)malloc(sizeof(Nivel2));
			Nivel3 *e3 = (Nivel3*)malloc(sizeof(Nivel3));


	e1->id = malloc(255);
	e2->id = malloc(255);
	e2->value = malloc(255);
	e3->id = malloc(255);
	e3->value = malloc(255);

	L_Crear(&e1->Level2,sizeof(Nivel2));

	L_Crear(&e2->Level3,sizeof(Nivel3));

	*e1->id = 0;
	*e2->id = 0;
	*e3->id = 0;
	*e3->value = 0;
	*e2->value = 0;

	/* Nivel 3 */
	strcpy(e3->id,"New");
	strcpy(e3->value,"CreateNewDoc()");

	L_Insertar_Cte(&e2->Level3,L_Siguiente,e3);

	strcpy(e3->id,"Open");
	strcpy(e3->value,"OpenDoc()");

	L_Insertar_Cte(&e2->Level3,L_Siguiente,e3);

	strcpy(e3->id,"Close");
	strcpy(e3->value,"CloseDoc()");

	L_Insertar_Cte(&e2->Level3,L_Siguiente,e3);
	/* Nivel 3 */

	/* Nivel 2 */
	strcpy(e2->id,"file");
	strcpy(e2->value,"File");

	L_Insertar_Cte(&e1->Level2,L_Siguiente,e2);
	/* Nivel 2 */

	/* Nivel 1 */
	strcpy(e1->id,"menu");
	/* Nivel 1 */

	/** Definicion de estructura */

	/* Estructura a Archivo */

	error = 0; /*xmlGuardar(&tc->xml, rutaXml);*/

	if (error != OK)
	{
		fprintf(stderr,"%d\n",error);
		return error;
	}

	fclose (arch);

	/* Estructura a Archivo */

	free(e1->id);
	L_Vaciar(&e1->Level2);
	free(e1);

	free(e2->id);
	free(e2->value);
	L_Vaciar(&e2->Level3);
	free(e2);

	free(e3->id);
	free(e3->value);
	free(e3);

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

TDAConvertidor *creartc()
{

	TDAConvertidor *tc = malloc(sizeof(TDAConvertidor));

	return tc;

}
