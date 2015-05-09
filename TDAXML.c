#include "TDAXML.h"
#include "Lista.h"
#include "TDAConvertidor.h"

#define TAM_ELEM 200

/* Funcion que escribe una cantidad de tabs en el archivo */
void EscribirTabs(int cant, FILE* arch)
{
	while (cant-- > 0)
		fputc('\t', arch);
}

/* Funcion que devuelve 0 si el elemento es un delimitador */
int EsDelimitador (TElem aux)
{
	return (aux.estado != Valor)?0:-1;
}

/* Funcion que escribe la apertura de un atributo en un archivo */
void EscribirAtributo(char* at, FILE* arch)
{
	fputc('<', arch);
	fputs(at, arch);
	fputc('>', arch);
}

int xmlCargar(TDAXML *TDAXml, char *rutaXml)
{

	int error;
	int cont;
	char letra;

	int tag = 0; /* para el tagpcpal */

	/*Malloc*/
	TElem Elem; /* donde voy guardando las letras */
	/*Malloc*/

	do
	{

		letra = fgetc(TDAXml->xmlFile);

	}while ((letra != '<') && (letra != EOF));

	while (letra != EOF)
	{

		Elem.id[0] = 0;

		if (letra == '<')
		{

			letra = fgetc(TDAXml->xmlFile);

			if (letra == EOF)
				break;

			cont = 0;

			while (letra != '>')
			{

				Elem.id[cont] = letra;

				letra = fgetc(TDAXml->xmlFile);

				if (letra == EOF)
					break;

				cont++;

			}

			Elem.id[cont] = 0;

			if (tag != 0)
			{

				if(Elem.id[0] == '/')
				{
					Elem.estado = Cerrado;
				}
				else
					Elem.estado = Abierto;

				error = L_Insertar_Cte(&TDAXml->atributos,L_Siguiente,&Elem);

			}
			else /* Solo la primera vez */
			{

				strcpy(TDAXml->tagPrincipal,Elem.id);

				tag = 1;

			}

		}
		else
		{

			if (letra == '>')
			{

				letra = fgetc(TDAXml->xmlFile);

				if (letra == EOF)
					break;

				cont = 0;

				while (letra != '<')
				{

					Elem.id[cont] = letra;

					letra = fgetc(TDAXml->xmlFile);

					if (letra == EOF)
						break;

					cont++;

				}

				Elem.id[cont] = '\0';

				if ((Elem.id[0] != '\n') && (Elem.id[0] != '\t') && (letra == '<'))
				{

					Elem.estado = Valor;

					error = L_Insertar_Cte(&TDAXml->atributos,L_Siguiente,&Elem);

				}

			}

		}

	}

	if (error == TRUE)
		error = 0;

	return error;

}

int xmlGuardar(TDAXML *TDAXml, char *rutaXml)
/*{
	TElem Aux;
	FILE *arch = fopen(rutaXml,"w");
	int code = 0;
	int nivel = 0; * Se utiliza para saber cuantos "tabs" imprimir en el archivo *
	int NoMueveCorriente = FALSE;
	if (arch == NULL) { * No se pudo abrir el archivo *
		return -1;
	}
	* Escribo el tagPrincipal *
	EscribirAtributo_Apertura(TDAXml->tagPrincipal,arch);
	fprintf(arch,"\n");
	EscribirTabs(++nivel, arch);
	* Comienzo a recorrer la lista de atributos *
	if (L_Vacia(TDAXml->atributos)!=0) {
		do {
			NoMueveCorriente = FALSE;
			code = L_Mover_Cte(&(TDAXml->atributos),L_Primero);
			L_Elem_Cte(TDAXml->atributos,&Aux);
			if (Aux.estado==Abierto){   * Es apertura *
				EscribirAtributo_Apertura(Aux.id, arch);
				L_Borrar_Cte(&TDAXml->atributos);
				* El corriente queda en el siguiente, cómo es apertura debe existir *
				L_Elem_Cte(TDAXml->atributos,&Aux);
				if (EsDelimitador(Aux)) {
                    			fprintf(arch, "\n");
                    			EscribirTabs(++nivel,arch);
                    			NoMueveCorriente = TRUE;
                		}
			}
			else {
                		if (Aux.estado==Cerrado) { * Es cierre *
                    			EscribirAtributo_Cierre(Aux.id, arch);
                    			fprintf(arch, "\n");
                    			EscribirTabs(--nivel,arch);
        			 }
                		if (Aux.estado==Valor) * Es un elemento *
                    			fputs(Aux.id, arch);
                	}
                	if (NoMueveCorriente == FALSE) * Debo mover el corriente  *
                		code = L_Mover_Cte(&(TDAXml->atributos),L_Siguiente);
		} while (code != 0);
	}
	* Cierro el tagPrincipal *
	EscribirAtributo_Cierre(TDAXml->tagPrincipal,arch);
	* Cierro el archivo *
	fclose(arch);
	* Destruyo la estructura TDAXML *
	free(TDAXml->tagPrincipal);
 	L_Vaciar(&TDAXml->atributos);
	return 0;
}*/

{

	TElem Aux;
	FILE *arch = fopen(rutaXml,"w");
	int code = 0;
	int nivel = 0; /* Se utiliza para saber cuantos "tabs" imprimir en el archivo */

	if (arch == NULL)
	{
		fprintf(stderr,"La ruta %s no es valida\n", rutaXml);
		return -1;
	}

	/* Escribo el tagPrincipal */
	EscribirAtributo(TDAXml->tagPrincipal,arch);
	fprintf(arch,"\n");
	EscribirTabs(++nivel, arch);

	/* Comienzo a recorrer la lista de atributos */
	code = L_Mover_Cte(&(TDAXml->atributos),L_Primero);

	while (code != FALSE)
	{

		L_Elem_Cte(TDAXml->atributos,&Aux);

		if (Aux.estado == Valor) /* Es un elemento */
		{
			fputs(Aux.id, arch);
			/*fprintf(arch,"\n");*/
		}
		else
		{

			EscribirAtributo(Aux.id,arch);

			if (Aux.estado == Abierto)
				/*EscribirTabs(++nivel,arch);*/
				nivel++;
			else
			{
				fputc('\n', arch);
				EscribirTabs(--nivel,arch);
			}

		}

		code = L_Mover_Cte(&(TDAXml->atributos),L_Siguiente);

	}

	/* Cierro el archivo */
	fclose(arch);

	return 0;

}

