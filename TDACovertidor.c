#include "TDACovertidor.h"
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

	Nivel1 *e11 = (Nivel1*)malloc(sizeof(Nivel1));
		Nivel2 *e21 = (Nivel2*)malloc(sizeof(Nivel2));
			Nivel3 *e31 = (Nivel3*)malloc(sizeof(Nivel3));
			Nivel3 *e32 = (Nivel3*)malloc(sizeof(Nivel3));
			Nivel3 *e33 = (Nivel3*)malloc(sizeof(Nivel3));

	e11->id = malloc(255);
	e21->id = malloc(255);
	e21->value = malloc(255);
	e31->id = malloc(255);
	e31->value = malloc(255);
	e32->id = malloc(255);
	e32->value = malloc(255);
	e33->id = malloc(255);
	e33->value = malloc(255);

	*e11->id = 0;
	*e21->id = 0;
	*e31->id = 0;
	*e32->id = 0;
	*e33->id = 0;
	*e33->value = 0;
	*e32->value = 0;
	*e31->value = 0;
	*e21->value = 0;

	/* Nivel 3 */
	strcpy(e31->id,"New");
	strcpy(e31->value,"CreateNewDoc()");

	strcpy(e32->id,"Open");
	strcpy(e32->value,"OpenDoc()");

	strcpy(e33->id,"Open");
	strcpy(e33->value,"OpenDoc()");
	/* Nivel 3 */

	/* Nivel 2 */
	strcpy(e21->id,"file");
	strcpy(e21->value,"File");

	L_Crear(&e21->Level3,sizeof(Nivel3));

	L_Insertar_Cte(&e21->Level3,L_Primero,e31);
	L_Insertar_Cte(&e21->Level3,L_Siguiente,e32);
	L_Insertar_Cte(&e21->Level3,L_Siguiente,e33);
	/* Nivel 2 */

	/* Nivel 1 */
	strcpy(e11->id,"menu");
	L_Crear(&e11->Level2,sizeof(Nivel2));
	L_Insertar_Cte(&e11->Level2,L_Primero,e21);
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

	free(e11->id);
	L_Vaciar(&e11->Level2);
	free(e11);

	free(e21->id);
	free(e21->value);
	L_Vaciar(&e21->Level3);
	free(e21);

	free(e31->id);
	free(e31->value);
	free(e31);

	free(e32->id);
	free(e32->value);
	free(e32);

	free(e33->id);
	free(e33->value);
	free(e33);

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
