#include "TDAXML.h"
#include "Lista.h"
#include "TDAConvertidor.h"

#define TAM_ELEM 200

/* Funcion que escribe una cantidad de tabs en el archivo */
void EscribirTabs_XML(int cant, FILE* arch)
{
	while (cant-- > 0)
		fputc('\t', arch);
}

/* Funcion que escribe la apertura de un atributo en un archivo */
void EscribirAtributo_Apertura(char at[CANTMAX], FILE* arch)
{
	fputc('<', arch);
	fputs(at, arch);
	fputc('>', arch);
}

/* Funcion que escribe el cierre de un atributo en un archivo */
void EscribirAtributo_Cierre(char at[CANTMAX], FILE* arch)
{
	fputc('<', arch);
	fputc('/', arch);
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

		cont = 0;

		while (cont != CANTMAX)
		{
			Elem.id[cont] = 0;
			cont++;
		}

		if (letra == '<')
		{

			letra = fgetc(TDAXml->xmlFile);

			if (letra == EOF)
				break;

			cont = 0;

			if(letra == '/')
			{
				Elem.estado = Cerrado;
				letra = fgetc(TDAXml->xmlFile);
			}
			else
				Elem.estado = Abierto;

			while (letra != '>')
			{

				Elem.id[cont] = letra;

				letra = fgetc(TDAXml->xmlFile);

				if (letra == EOF)
					break;

				cont++;

			}

			if (tag != 0)
			{

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

int xmlGuardar(TDAJSON *TDAXml, char *rutaXml)
{
	TElem Aux;
	FILE *arch = fopen(rutaXml,"w");
	int code = 0;
	int nivel = 0; /* Se utiliza para saber cuantos "tabs" imprimir en el archivo */
	int LeerElemento = TRUE;
	if (arch == NULL) /* No se pudo abrir el archivo */
		return -1;
	/* Escribo el tagPrincipal */
	EscribirAtributo_Apertura(TDAXml->tagPrincipal,arch);
	fputc('\n', arch);
	EscribirTabs_XML(++nivel, arch);
	/* Comienzo a recorrer la lista de atributos */
	if (L_Vacia(TDAXml->atributos)==0) {
		code = L_Mover_Cte(&(TDAXml->atributos),L_Primero);
		do {
			if (LeerElemento == TRUE)
				L_Elem_Cte(TDAXml->atributos,&Aux);
			else
				LeerElemento = TRUE;
			switch(Aux.estado)
			{
				case(Abierto): /* Es apertura */
				{
					EscribirAtributo_Apertura(Aux.id, arch);
					code = L_Mover_Cte(&(TDAXml->atributos),L_Siguiente);
					/* El corriente queda en el siguiente, cómo es apertura debe existir */
					L_Elem_Cte(TDAXml->atributos,&Aux);
					LeerElemento = FALSE;
					if (Aux.estado == Abierto) {
						fputc('\n', arch);
						EscribirTabs_XML(++nivel,arch);
					}
					break;
				}
				case(Cerrado): /* Es cierre */
				{
					EscribirAtributo_Cierre(Aux.id, arch);
					fputc('\n', arch);
					code = L_Mover_Cte(&(TDAXml->atributos),L_Siguiente);
					if (code != 0) {
						L_Elem_Cte(TDAXml->atributos,&Aux);
						LeerElemento = FALSE;
						if (Aux.estado == Cerrado)
							EscribirTabs_XML(--nivel,arch);
						else /* Es Apertura */
							EscribirTabs_XML(nivel,arch);
					}
					break;
				}
				case(Valor):
				{
					fputs(Aux.id, arch);
					code = L_Mover_Cte(&(TDAXml->atributos),L_Siguiente);
					break;
				}
				default:
					break;
			}
		} while (code != 0);
	}
	/* Cierro el tagPrincipal */
	EscribirAtributo_Cierre(TDAXml->tagPrincipal,arch);
	/* Cierro el archivo */
	fclose(arch);
	return 0;
}

void xmlDestruir(TDAXML* TDAXml)
{
	free(TDAXml->tagPrincipal);
	L_Vaciar(&(TDAXml->atributos));
}
