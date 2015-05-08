#include "TDAConvertidor.h"
#include "TDAJSON.h"
#include "TDAXML.h"

#define OK 0

int xml2json(TDAConvertidor *tc, char *rutaXml, char *rutaJson)
{

	int error;

	/*Esto vuela*/
	TElem Elem;
	/*Esto vuela*/

	/*Malloc*/
	tc = malloc(sizeof(TDAConvertidor));

	if ((tc->xml.tagPrincipal = malloc(255)) == NULL)
		return -1;

	if ((tc->xml.xmlFile = fopen(rutaXml, "r")) == NULL)
	{
		fprintf(stderr,"La ruta %s no es valida\n", rutaXml);
		return -2;
	}

	L_Crear(&(tc->xml.atributos),sizeof(TElem));
	/*Malloc*/

printf("Esto quiero subir a la lista:\n\n"); /*Esto tambien*/

	error = xmlCargar(&tc->xml, rutaXml);

	if (error != OK)
	{
		fprintf(stderr,"%d\n",error);
		return error;
	}

	if (L_Vacia(tc->xml.atributos) == FALSE)
	{

		/*error = jsonGuardar(&tc->json, rutaJson);*/


		/*Esto tambien vuela*/
printf("\n\nEsto tengo en la lista:\n\n");

		error = L_Mover_Cte(&tc->xml.atributos,L_Primero);

		while (error == TRUE)
		{

			L_Elem_Cte(tc->xml.atributos,&Elem);

			if (Elem.estado == Valor)
				printf("Valor = %s\n", Elem.id);
			if (Elem.estado == Abierto)
				printf("EL Delim ES: %s (Abierto)\n", Elem.id);
			if (Elem.estado == Cerrado)
				printf("EL Delim ES: %s (Cerrado)\n", Elem.id);

			error = L_Mover_Cte(&tc->xml.atributos,L_Siguiente);

		}
		/*Esto tambien vuela*/


		error = xmlGuardar(&tc->xml, rutaJson);

		if (error != OK)
		{
			fprintf(stderr,"%d\n",error);
			return error;
		}

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
	/*tc = (TDAConvertidor*)malloc(sizeof(TDAConvertidor));
	if (tc == NULL)
		return -1;*/

	printf("\n"); /*No deberia ser así*/
	return 0;
}

void DestruirTC(TDAConvertidor *tc)
{
	/*L_Vaciar(&tc->xml.atributos);
	free(tc->xml.tagPrincipal);
	free(tc);*/

	printf("\n"); /*No deberia ser así*/
}
