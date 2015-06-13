#include "TDAXML.h"
#include "Lista.h"
#include "TDAConvertidor.h"

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

	TElem Elem; /* donde voy guardando las letras */

	do
	{

		letra = fgetc(TDAXml->xmlFile);

	}while ((letra != '<') && (letra != EOF)); /* Leo hasta que encuentro < o el fin del archivo */

	while (letra != EOF)
	{

		Elem.id[0] = '\0';

		if (letra == '<') /* si encuentro < tengo un id */
		{

			letra = fgetc(TDAXml->xmlFile);

			if (letra == EOF)
				break;

			cont = 0;

			if (letra == '/') /* si empieza con / significa que está siendo cerrado */
			{
				Elem.estado = Cerrado;
				letra = fgetc(TDAXml->xmlFile);
			}
			else
				Elem.estado = Abierto;

			while ((cont < CANTMAX) && (letra != '>')) /* Leo y voy guardando hasta que encuentre > */
			{

				Elem.id[cont] = letra;

				letra = fgetc(TDAXml->xmlFile);

				if (letra == EOF)
					break;

				cont++;

			}

			if (cont == CANTMAX)
				while (letra != '>')
					letra = fgetc(TDAXml->xmlFile);

			Elem.id[cont] = '\0';

			if (tag != 0)
			{

				error = L_Insertar_Cte(&TDAXml->atributos,L_Siguiente,&Elem); /* Guardo en Lista */

			}
			else /* Solo la primera vez */
			{

				strcpy(TDAXml->tagPrincipal,Elem.id); /* Guardo en tagPrincipal */

				tag = 1;

			}

		}
		else
		{

			if (letra == '>') /* Si tengo > puede empezar un valor */
			{

				letra = fgetc(TDAXml->xmlFile);

				if (letra == EOF)
					break;

				cont = 0;

				while ((cont < CANTMAX) && (letra != '<')) /* Leo y voy guardando hasta que encuentre < */
				{

					Elem.id[cont] = letra;

					letra = fgetc(TDAXml->xmlFile);

					if (letra == EOF)
						break;

					cont++;

				}

				if (cont == CANTMAX)
					while (letra != '<')
						letra = fgetc(TDAXml->xmlFile);

				Elem.id[cont] = '\0';

				if ((Elem.id[0] != '\n') && (Elem.id[0] != '\t') && (letra == '<'))
				{

					Elem.estado = Valor;

					error = L_Insertar_Cte(&TDAXml->atributos,L_Siguiente,&Elem);

				}

			}

		}

	}

	L_Borrar_Cte(&TDAXml->atributos); /* Elimino el cierre de tagPrincipal */

	fclose(TDAXml->xmlFile);

	return error;

}

int xmlGuardar(TDAXML *TDAXml, char *rutaXml)
{

	TElem Aux;
	FILE *arch;

	int code = 0;
	int nivel = 0; /* Se utiliza para saber cuantos "tabs" imprimir en el archivo */
	int LeerElemento = TRUE;

	/* Comienzo a recorrer la lista de atributos */
	if (L_Vacia(TDAXml->atributos) == 0)
	{

		arch = fopen(rutaXml,"w");

		if (arch == NULL) /* No se pudo abrir el archivo */
			return -2;

		/* Escribo el tagPrincipal */
		EscribirAtributo_Apertura(TDAXml->tagPrincipal,arch);
		fputc('\n', arch);
		EscribirTabs_XML(++nivel, arch);

		code = L_Mover_Cte(&(TDAXml->atributos),L_Primero);
		do
		{

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

					if (Aux.estado == Abierto)
					{
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

					if (code != 0)
					{
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
		} while (code != FALSE);

		/* Cierro el tagPrincipal */
		EscribirAtributo_Cierre(TDAXml->tagPrincipal,arch);

		/* Cierro el archivo */
		fclose(arch);

	}
	else
		return -4;

	return TRUE;

}
