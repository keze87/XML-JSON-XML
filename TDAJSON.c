#include "TDAJSON.h"
#include "Lista.h"
#include "TDAConvertidor.h"

#define TAM_ELEM 200

#define DELIM_FIN ": {" /* String luego de un DELIM */
#define ELEM_SEPARADOR ',' /* Caracter que separa múltiples elementos */
#define ELEM_SEPARADOR_ATR_VAL ": " /* String que separa el Atributo y el Value de un elemento */
#define TAB "    "

/* Funcion que se encarga de escribir un atributo */
void EscribirAtributo(char at[CANTMAX], FILE* arch)
{
	fputc('\"', arch);
	fputs(at, arch);
	fputc('\"', arch);
}

/* Funcion que escribe una cantidad de tabs en el archivo */
void EscribirTabs_JSON(int nivel, int cant, FILE* arch)
{
	nivel *= 2;
	while (cant-- > 0)
        fputc(' ', arch);
	while (nivel-- > 0)
		fputs(TAB, arch);
}

/* Funcion que se encarga de escribir los cierres finales */
void EscribirCierre(int nivel, int long_atributo, FILE* arch)
{
	EscribirTabs_JSON(nivel,long_atributo - (nivel - 1), arch);
	fputs(TAB, arch);
	fputc('}', arch);
}

void FinalizarCorchete(int *CA, int *CAN2, int* PC, FILE* arch)
{
	fputc(']', arch);
	if (*CAN2 == TRUE) /* Dos niveles de corchetes */
		*CAN2 = FALSE;
	else
		*CA = FALSE;
	*PC = TRUE;
}

void CerrarTagPrincipal(int LongAtributo, FILE* arch)
{
	EscribirTabs_JSON(0, LongAtributo, arch);
	fputs(TAB, arch);
	fputc(' ', arch);
	fputc('}', arch);
	fputc ('\n', arch);
	fputc('}', arch);
}


int jsonCargar(TDAJSON *TDAJson, char *rutaJson)
{

	TElem Elem;

	char letra;
	char letra_ant;
	char palabra_ant[CANTMAX]; /*la uso por si se repite con el corchete*/

	int i = 0, tope = -1, error = 0;
	int primero_multiple = TRUE; /*flag para saber si es la primera vez que se escribe un atributo múltiple*/
	char vector[4][CANTMAX];

	/*inicializo*/
	vector[0][0] = '\0';
	vector[1][0] = '\0';
	vector[2][0] = '\0';
	vector[3][0] = '\0';

	Elem.id[0] = 0; /* palabra */

	if (!TDAJson->jsonFile)
	{

		fprintf(stderr,"La ruta %s no es valida\n", rutaJson);

		return 1;

	}

	letra = fgetc(TDAJson->jsonFile);

	/*ANTES DE GUARDAR EN LA LISTA, HAY QUE CHEQUEAR SIEMPRE QUE EL ID NO SEA NULO*/

	while (letra != EOF) /*mientras que no se termine el archivo*/
	{

		/*SI ESTA ENTRE COMILLAS*/
		if ((letra == 34)||(letra >= 48 && letra <= 57)) /*COMILLA o NUMERO*/ /*recorro hasta que cierre la comilla*/
		{

			if (letra == 34)
			{
				letra = fgetc(TDAJson->jsonFile);

				while(letra != 34) /*mientras no cierre comilla, leo y guardo en Elem.id*/
				{
					Elem.id[i] = letra;
					i++;
					letra = fgetc(TDAJson->jsonFile);
				}

				Elem.id[i] = '\0';
				strcpy(palabra_ant, Elem.id);
			}

			if (letra >= 48 && letra <= 57) /*si es numero*/
			{
				while(letra >= 48 && letra <= 57) /*mientras que sean numeros, leo y guardo en Elem.id*/
				{
					Elem.id[i] = letra;
					i++;
					letra = fgetc(TDAJson->jsonFile);
				}

				Elem.id[i] = '\0';
				Elem.estado=Valor; /*si es un numero, solo puede ser Valor, no puede ser ni Abierto ni Cerrado*/

				if (Elem.id[0] != '\0')
					error = L_Insertar_Cte(&TDAJson->atributos,L_Siguiente,&Elem);

				/*si guarde un numero como Valor, tengo que cerrar lo anterior que abri*/
				strcpy(Elem.id,vector[tope]);
				Elem.estado = Cerrado;

				if (Elem.id[0] != '\0')
					error = L_Insertar_Cte(&TDAJson->atributos,L_Siguiente,&Elem);

				vector[tope][0] = '\0';
				if (tope > 0)
					tope--;
			}

			strcpy(palabra_ant, Elem.id);

			/*obtengo una letra mas y me fijo si es un ":"*/
			letra = fgetc(TDAJson->jsonFile);
			while (letra==32 || letra==10 || letra==9) /*mientras que no sea espacio o enter o tab*/
                		letra = fgetc(TDAJson->jsonFile);

			if (letra == ':')
			{
				tope++;

				strcpy(vector[tope], Elem.id);

				Elem.estado = Abierto;
				if (Elem.id[0] != '\0')
					error = L_Insertar_Cte(&TDAJson->atributos,L_Siguiente,&Elem);
			}

			if ((letra == ',') || (letra == '}'))
			{
				Elem.estado = Valor;
				if (Elem.id[0] != '\0')
					error = L_Insertar_Cte(&TDAJson->atributos,L_Siguiente,&Elem);

				strcpy(Elem.id,vector[tope]);
				Elem.estado = Cerrado;
				if (Elem.id[0] != '\0')
					error = L_Insertar_Cte(&TDAJson->atributos,L_Siguiente,&Elem);

				vector[tope][0] = '\0';
				if (tope > 0)
					tope--;
			}

			Elem.id[0] = '\0'; /*limpio la variable*/
			i = 0;

		}

		if (letra == '}')
		{
			strcpy(Elem.id,vector[tope]);
			Elem.estado = Cerrado;

			if (Elem.id[0] != '\0')
				error = L_Insertar_Cte(&TDAJson->atributos,L_Siguiente,&Elem);

			vector[tope][0] = '\0';
			if (tope > 0)
				tope--;
		}

		if (letra == '[')
		{
			vector[tope][0] = '\0'; /*borro el multiple para no repetirlo en el cierre*/
			if (tope > 0)
				tope--;

			while(letra != ']') /*mientras no cierre corchete*/
			{

				if (letra == 34) /*COMILLA*/ /*recorro hasta que cierre la comilla*/
				{

					letra_ant = letra;
					letra = fgetc(TDAJson->jsonFile);

					while (letra != 34) /*mientras no cierre comilla, leo y guardo en Elem.id*/
					{
						Elem.id[i] = letra;
						i++;
						letra_ant = letra;
						letra = fgetc(TDAJson->jsonFile);
					}

					Elem.id[i] = '\0';

					letra_ant = letra;
					letra = fgetc(TDAJson->jsonFile);
                    			while (letra==32 || letra==10 || letra==9) /*mientras que no sea espacio o enter o tab*/
                        			letra = fgetc(TDAJson->jsonFile);
                        
					if(letra == '}') /*si cierra la llave, tengo que cerrar y volver a abrir el id*/
					{

						Elem.estado = Valor;
						if (Elem.id[0] != '\0')
							error = L_Insertar_Cte(&TDAJson->atributos,L_Siguiente,&Elem);

						strcpy(Elem.id,vector[tope]);
						Elem.estado = Cerrado;
						if (Elem.id[0] != '\0')
							error = L_Insertar_Cte(&TDAJson->atributos,L_Siguiente,&Elem);

						vector[tope][0] = '\0';
						if (tope > 0)
							tope--;

						strcpy(Elem.id,palabra_ant);
						Elem.estado = Cerrado;
						if (Elem.id[0] != '\0')
							error = L_Insertar_Cte(&TDAJson->atributos,L_Siguiente,&Elem);

						primero_multiple = FALSE;

					}

					if (letra == ':')
					{
						tope++;
						strcpy(vector[tope], Elem.id);

						Elem.estado = Abierto;
						if (Elem.id[0] != '\0')
							error = L_Insertar_Cte(&TDAJson->atributos,L_Siguiente,&Elem);
					}

					if ((letra == ',') && (letra_ant != '}'))
					{
						Elem.estado = Valor;
						if (Elem.id[0] != '\0')
							error = L_Insertar_Cte(&TDAJson->atributos,L_Siguiente,&Elem);

						strcpy(Elem.id,vector[tope]);
						Elem.estado = Cerrado;
						if (Elem.id[0] != '\0')
							error = L_Insertar_Cte(&TDAJson->atributos,L_Siguiente,&Elem);

						vector[tope][0] ='\0';
						if (tope > 0)
							tope--;
					}

					Elem.id[0] = '\0'; /*limpio la variable*/
					i = 0;

				}

				if ((letra == '{') && (primero_multiple == FALSE))
				{
					strcpy(Elem.id,palabra_ant);
					Elem.estado = Abierto;

					if (Elem.id[0] != '\0')
						error = L_Insertar_Cte(&TDAJson->atributos,L_Siguiente,&Elem);
				}

				letra_ant = letra;
				letra = fgetc(TDAJson->jsonFile);
        			while (letra==32 || letra==10 || letra==9) /*mientras que no sea espacio o enter o tab*/
                    			letra = fgetc(TDAJson->jsonFile);

			}

		}

		letra = fgetc(TDAJson->jsonFile);
		while (letra==32 || letra==10 || letra==9) /*mientras que no sea espacio o enter o tab*/
			letra = fgetc(TDAJson->jsonFile);

	}

	Elem.id[i] = '\0';
	i++;

	fclose(TDAJson->jsonFile);

	L_Borrar_Cte(&TDAJson->atributos); /*borro el ultimo, que es el cierre del tag principal*/

	error = L_Mover_Cte(&TDAJson->atributos, L_Primero);
	L_Elem_Cte(TDAJson->atributos, &Elem);
	strcpy(TDAJson->tagPrincipal, Elem.id); /*el primer elemento de la lista será el tag principal*/
	L_Borrar_Cte(&TDAJson->atributos); /*borro el primero, que es la apertura del tag principal*/

	return error;

}

int jsonGuardar(TDAJSON *TDAJson, char *rutaJson)
{
	TElem Aux;
	FILE *arch;
	char Atr_Aux[CANTMAX];
	int code = 0;
	int nivel = 0; /* Se utiliza para saber cuantos tabs imprimir en el archivo */
	/* Defino booleanos a utilizar */
	int EsCerrado = FALSE;
	int CoincidenAtributos = FALSE;
	int CoincidenAtributos_Nivel2 = FALSE;
	int PrimerCoincidencia = TRUE;
	/* Defino variables auxiliares */
	int nivel_aux = 0; /* Se utiliza para almacenar el nivel de Atr_Aux */
	int LongAtributo = 0; /* Acumula la longitud de los atributos cuando existen corchetes, por diseño */
	/* Acumula la longitud de los atributos cuando deben escribirse llaves
	 * Cada posición corresponde a un nivel, en total son 3 niveles */
	int LongAtributo_Llave[3];
	char Atributo_Llave[CANTMAX];
	char Atributo_Llave_Nivel2[CANTMAX];
	/* Acumula los atributos cuando deben escribirse corchetes */
	if (L_Vacia(TDAJson->atributos) == TRUE)
		return -4;
	Atributo_Llave[0] = '\0';
	Atributo_Llave_Nivel2[0] = '\0';
	Atr_Aux[0] = '\0';
	if ((arch = fopen(rutaJson, "w")) == NULL) /* No se pudo abrir el archivo */
		return -2;
	/* Escribo el tagPrincipal */
	fputc('{', arch);
	EscribirAtributo(TDAJson->tagPrincipal,arch);
	fputs(DELIM_FIN, arch);
	fputc('\n', arch);
	LongAtributo += (strlen(TDAJson->tagPrincipal));
	LongAtributo_Llave[nivel] = LongAtributo;
	EscribirTabs_JSON(++nivel, LongAtributo, arch);
	/* Comienzo a recorrer la lista de atributos */
	code = L_Mover_Cte(&(TDAJson->atributos),L_Primero);
	L_Elem_Cte(TDAJson->atributos,&Aux);
	while (L_Vacia(TDAJson->atributos) == 0)
	{
		switch (Aux.estado)
		{
			case(Abierto): /* Es apertura */
			{
				if ((CoincidenAtributos == TRUE) && (PrimerCoincidencia == FALSE)) /* Estoy dentro del corchete y no es el primero */
				{
					fputc(ELEM_SEPARADOR, arch);
					fputc(' ', arch);
				}
				EscribirAtributo(Aux.id, arch);
				fputs(ELEM_SEPARADOR_ATR_VAL, arch);
				/* Guardo el nombre del atributo y el nivel para buscar si se repite */
				strcpy(Atr_Aux,Aux.id);
				nivel_aux = nivel;
				L_Borrar_Cte(&(TDAJson->atributos));
				/* El corriente queda en el siguiente, cómo es apertura debe existir */
				L_Elem_Cte(TDAJson->atributos,&Aux);
				if (Aux.estado == Abierto) /* Es otro de apertura, ya que no puede ser cerrado, debe tener valor */
				{
					nivel++;
					/* Recorro la lista hasta encontrar el primer elemento del mismo nivel */
					EsCerrado = FALSE;
					while ((code != 0) && (nivel_aux < nivel))
					{
						code = L_Mover_Cte(&(TDAJson->atributos),L_Siguiente);
						L_Elem_Cte(TDAJson->atributos,&Aux);
						switch(Aux.estado)
						{
							case(Abierto):
							{
								if (EsCerrado == FALSE)
									nivel++;
								else
									EsCerrado = FALSE;
								break;
							}
							case(Cerrado):
							{
								nivel--;
								EsCerrado = TRUE;
								break;
							}
							case(Valor):
							{
								nivel++;
								break;
							}
							default: break;
						}
					}
					/* El corriente se encuentra en el mismo nivel, pero en el Cerrado con id = Atr_Aux */
					code = L_Mover_Cte(&(TDAJson->atributos),L_Siguiente);
					if (code != 0) /* Existe otro elemento del mismo nivel */
					{
						L_Elem_Cte(TDAJson->atributos,&Aux);
						if (strcmp(Atr_Aux,Aux.id) == 0) /* Coinciden los atributos */
						{
							if ((CoincidenAtributos == TRUE) && (nivel == 1)) /* Ya estoy dentro de un corchete */
                                				nivel++;
							LongAtributo_Llave[nivel] = strlen(Aux.id);
							LongAtributo += LongAtributo_Llave[nivel];
							if (CoincidenAtributos == TRUE)
							{
								CoincidenAtributos_Nivel2 = TRUE; /* Un corchete adentro de otro */
								strcpy(Atributo_Llave_Nivel2,Aux.id);
							}
							else
								strcpy(Atributo_Llave,Aux.id);
							fputc('[', arch);
							fputc('{', arch);
							CoincidenAtributos = TRUE;
							PrimerCoincidencia = TRUE;
						}
						else /* No coinciden los atributos */
						{
							fputc('{', arch);
							fputc('\n', arch);
							LongAtributo_Llave[nivel] = (strlen(Atr_Aux));
							LongAtributo += LongAtributo_Llave[nivel];
							nivel++;
							EscribirTabs_JSON(nivel,(LongAtributo - (nivel - 1)), arch);
							/* Se resta nivel -1 porque en cada nivel se resta un espacio */
						}
					}
					else /* Es el último elemento de su nivel */
					{
						fputc('{', arch);
						fputc('\n', arch);
						LongAtributo_Llave[nivel] = (strlen(Atr_Aux));
						LongAtributo += LongAtributo_Llave[nivel];
						nivel++;
						EscribirTabs_JSON(nivel,LongAtributo - (nivel - 1), arch);
					}
					code = L_Mover_Cte(&(TDAJson->atributos),L_Primero); /* Vuelvo al atributo posterior al repetido */
					L_Elem_Cte(TDAJson->atributos,&Aux);
				}
				break;
			}
			case(Cerrado): /* Es un cierre */
			{
				if (CoincidenAtributos == FALSE) /* No estoy dentro de los corchetes */
				{
                    			L_Borrar_Cte(&(TDAJson->atributos));
                    			if (L_Vacia(TDAJson->atributos) == 0)
					{
						L_Elem_Cte(TDAJson->atributos,&Aux);
						if (Aux.estado == Cerrado) {
                               				fputc('\n', arch);
                               				EscribirCierre(--nivel, LongAtributo, arch);
                               				LongAtributo -= LongAtributo_Llave[nivel];
                        			}
                        			else { /* Viene otro de apertura */
                            				fputc(ELEM_SEPARADOR, arch);
                            				fputc('\n', arch);
                            				EscribirTabs_JSON(nivel,LongAtributo - (nivel - 1),arch);
                        			}
					}
					else
					{
						fputc('\n', arch);
						CerrarTagPrincipal(LongAtributo, arch);
					}
				}
				else /* Estoy dentro de los corchetes */
				{
					L_Borrar_Cte(&(TDAJson->atributos));
					L_Elem_Cte(TDAJson->atributos,&Aux);
					if (Aux.estado == Cerrado) /* Termina la línea del corchete */
					{
						fputc('}', arch);
						L_Borrar_Cte(&(TDAJson->atributos));
						L_Elem_Cte(TDAJson->atributos,&Aux);
						switch (Aux.estado)
						{
							case(Cerrado): /* Termina el corchete */
							{
								FinalizarCorchete(&CoincidenAtributos,&CoincidenAtributos_Nivel2,&PrimerCoincidencia,arch);
								if (CoincidenAtributos == TRUE) /* Eran dos niveles de corchetes */
								{
									LongAtributo -= strlen(Atributo_Llave_Nivel2); /* Puramente por diseño */
									L_Borrar_Cte(&(TDAJson->atributos));
									if (L_Vacia(TDAJson->atributos) == 0) /* Aún existe un elemento más */
									{
										L_Elem_Cte(TDAJson->atributos,&Aux);
										if (strcmp(Atributo_Llave,Aux.id) == 0) /* El corriente corresponde al Abierto del primer nivel de corchete */
										{
											L_Borrar_Cte(&(TDAJson->atributos));
											L_Elem_Cte(TDAJson->atributos,&Aux);
											fputc(ELEM_SEPARADOR, arch);
										}
										else
										{
											fputc(']', arch);
											fputc(ELEM_SEPARADOR, arch);
											CoincidenAtributos = FALSE;
										}
										fputc('\n', arch);
                                        					EscribirTabs_JSON(nivel, LongAtributo - 2, arch);
									}
									else { /* No existian más elementos */
										fputc('}', arch);
										fputc(']', arch);
										nivel--;
										LongAtributo -= LongAtributo_Llave[nivel];
										fputc('\n', arch);
										CerrarTagPrincipal(LongAtributo, arch);
                                    					}
								}
								else
								{
									LongAtributo -= strlen(Atributo_Llave);
									fputc('\n', arch);
									nivel--;
									EscribirCierre(nivel, LongAtributo - (nivel - 1), arch);
									LongAtributo -=  LongAtributo_Llave[nivel];
								}
								break;
							}
							case (Abierto): /* Termina el corchete y viene otro atributo, o empieza otra linea*/
							{
								if (strcmp(Aux.id,(CoincidenAtributos_Nivel2 == FALSE)?Atributo_Llave:Atributo_Llave_Nivel2) == 0)
								{
									/* Coinciden los atributos */
									fputc(ELEM_SEPARADOR, arch);
									fputc('\n', arch);
									if (CoincidenAtributos_Nivel2 == TRUE)
                                        					EscribirTabs_JSON(nivel, LongAtributo - (nivel), arch);
                                    					else
                                        					EscribirTabs_JSON(nivel, LongAtributo - (nivel - 1), arch);
									fputs(TAB, arch);
									fputc('{', arch);
									PrimerCoincidencia = TRUE;
									/* El corriente es otra vea Atr_Aux Abierto */
									L_Borrar_Cte(&(TDAJson->atributos));
									L_Elem_Cte(TDAJson->atributos,&Aux);
								}
								else /* No coincide el atributo, es uno diferente */
								{
									FinalizarCorchete(&CoincidenAtributos,&CoincidenAtributos_Nivel2,&PrimerCoincidencia,arch);
									fputc(ELEM_SEPARADOR, arch);
									fputc('\n', arch);
									LongAtributo -= LongAtributo_Llave[nivel];
									EscribirTabs_JSON(nivel, LongAtributo - (nivel - 1), arch);
									break;
								}
								break;
							}
							default: break;
						}
					}
				}
				break;
			}
			case(Valor): /* Es un valor */
			{
				EscribirAtributo(Aux.id, arch);
				L_Borrar_Cte(&(TDAJson->atributos));
				if (PrimerCoincidencia == TRUE)
					PrimerCoincidencia = FALSE;
				L_Elem_Cte(TDAJson->atributos,&Aux);
				break;
			}
			default: break;
		}
	}
	/* Cierro el archivo */
	fclose(arch);
	return TRUE;
}
