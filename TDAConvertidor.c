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

int guardarxml (char *rutaXml)
{

	TElem* Elem = (TElem*)malloc(sizeof(TElem));
	TDelimitador* Delimitador = (TDelimitador*)malloc(sizeof(TDelimitador));
	TDAXML* xml = (TDAXML*)malloc(sizeof(TDAXML));

	xml->xmlFile = fopen(rutaXml,"w");

	if (!xml->xmlFile)
	{
		free(xml->xmlFile);
		free(xml);
		fprintf(stderr,"La ruta %s no es valida\n", rutaXml);
		return 1;
	}

	/*Malloc*/
	xml->tagPrincipal = malloc(255);
	Elem->id = malloc(255);
	Elem->value = malloc(255);
	Delimitador->id = malloc(255);
	Delimitador->estado = (TInterruptor)malloc(sizeof(TInterruptor));
	L_Crear(&xml->atributos,sizeof(TElem));
	/*Malloc*/

	/**Nivel1*/
	Delimitador->estado = Abierto;
	strcpy(Delimitador->id,"menu");

	L_Insertar_Cte(&xml->atributos,L_Siguiente,Delimitador);
	/**Nivel1*/

	/**Nivel2*/
	Delimitador->estado = Abierto;
	strcpy(Delimitador->id,"popup");

	L_Insertar_Cte(&xml->atributos,L_Siguiente,Delimitador);

	strcpy(Elem->id,"file");
	strcpy(Elem->value,"File");

	L_Insertar_Cte(&xml->atributos,L_Siguiente,Elem);
	/**Nivel2*/

	/**Nivel3*/
	/*Elemento1*/
	Delimitador->estado = Abierto;
	strcpy(Delimitador->id,"menuitem");

	L_Insertar_Cte(&xml->atributos,L_Siguiente,Delimitador);

	strcpy(Elem->id,"New");
	strcpy(Elem->value,"CreateNewDoc()");

	L_Insertar_Cte(&xml->atributos,L_Siguiente,Elem);

	Delimitador->estado = Cerrado;
	strcpy(Delimitador->id,"menuitem");

	L_Insertar_Cte(&xml->atributos,L_Siguiente,Delimitador);
	/*Elemento1*/

	/*Elemento2*/
	Delimitador->estado = Abierto;
	strcpy(Delimitador->id,"menuitem");

	L_Insertar_Cte(&xml->atributos,L_Siguiente,Delimitador);

	strcpy(Elem->id,"Open");
	strcpy(Elem->value,"OpenDoc()");

	L_Insertar_Cte(&xml->atributos,L_Siguiente,Elem);

	Delimitador->estado = Cerrado;
	strcpy(Delimitador->id,"menuitem");

	L_Insertar_Cte(&xml->atributos,L_Siguiente,Delimitador);
	/*Elemento2*/

	/*Elemento3*/
	Delimitador->estado = Abierto;
	strcpy(Delimitador->id,"menuitem");

	L_Insertar_Cte(&xml->atributos,L_Siguiente,Delimitador);

	strcpy(Elem->id,"Close");
	strcpy(Elem->value,"CloseDoc()");

	L_Insertar_Cte(&xml->atributos,L_Siguiente,Elem);

	Delimitador->estado = Cerrado;
	strcpy(Delimitador->id,"menuitem");

	L_Insertar_Cte(&xml->atributos,L_Siguiente,Delimitador);
	/*Elemento2*/
	/**Nivel3*/

	/**Nivel2*/
	Delimitador->estado = Cerrado;
	strcpy(Delimitador->id,"popup");

	L_Insertar_Cte(&xml->atributos,L_Siguiente,Delimitador);
	/**Nivel2*/

	/**Nivel1*/
	Delimitador->estado = Cerrado;
	strcpy(Delimitador->id,"menu");

	L_Insertar_Cte(&xml->atributos,L_Siguiente,Delimitador);
	/**Nivel1*/

	L_Vaciar(&xml->atributos);
	free(Elem->id);
	free(Elem->value);
	free(Elem);
	free(Delimitador->id);
	free(Delimitador);

	return 0;

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

	/*tc->xml = (TDAXML*)malloc(sizeof(TDAXML)); NO FUNCIONA :@*/
	/*tc->json = (TDAJSON*)malloc(sizeof(TDAJSON)); NO FUNCIONA :@*/

	return tc;

}
