#include "TDAXML.h"
#define CANTMAX 255 /*tamaño maximo de linea*/

int xmlCargar(TDAXML *TDAXml, char *rutaXml)
{

	FILE *archivoxml;

	FILE *values;
	FILE *ids;

	char linea; /*una letra*/

	L_Crear(&TDAXml->atributos,CANTMAX);

	archivoxml = fopen(rutaXml,"r");

	values = fopen("values.DAT","w");
	ids = fopen("ids.DAT","w");

	if ((archivoxml) && (values) && (ids))
	{

		TDAXml->xmlFile = archivoxml;

	}
	else
	{

		fprintf(stderr,"La ruta %s no es valida\n", rutaXml);

		return 1;

	}

	do
	{

		linea = fgetc(archivoxml);

	}while ((linea != 60) && (linea != 62) && (linea != EOF));

	while (archivoxml)
	{

		if (linea == 60) /* < */
		{

			linea = fgetc(archivoxml);

			if (linea == EOF)
				break;

			while (linea != 62) /* > */
			{

				fputc(linea,ids);

				linea = fgetc(archivoxml);

				if (linea == EOF)
					break;

			}

			fputc(10,ids); /* \n */

		}

		if (linea == 62) /* > */
		{

			linea = fgetc(archivoxml);

			if (linea == EOF)
				break;

			while (linea != 60) /* < */
			{

				if ((linea != 10/* \n */) && (linea != 9/* TAB */))
					fputc(linea,values);

				linea = fgetc(archivoxml);

				if (linea == EOF)
					break;

			}

			fputc(10,values); /* \n */

		}

		if (linea == EOF)
			break;

	}

	fclose(ids);
	fclose(values);

	return 0;

}

int xmlGuardar(TDAXML *TDAXml, char *rutaXml)
{

	FILE *archivoxml;
	archivoxml = fopen(rutaXml, "w");

	if (archivoxml)
	{

		/* Escribir la estructura a xml */

		fclose(archivoxml);
		printf("GUARDO XML\n");

	}
	else
		fprintf(stderr,"La ruta %s no es valida\n", rutaXml);

	return 0;

}
