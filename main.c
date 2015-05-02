#include <stdio.h>
#include <string.h>
#include "TDAConvertidor.h"

#define MAX_LONG_RUTA 100
#define C_NULO '\0'

#define OUT_OF_MEM "No existe suficiente memoria.\n"
#define RUTA_ORIGEN "Debe ingresar una ruta de origen a la que se pueda acceder para lectura.\n Debe ubicarse seguido de la operación a realizar.\n"
#define RUTA_DESTINO "Debe ingresar una ruta de destino a la que se pueda acceder para escritura.\n Debe ubicarse seguido de la ruta de origen.\n"

typedef enum {
    VACIO, /* Aún no se detectó la operación */
    XML2JSON, /* Convertir XML a JSON */
    JSON2XML, /* Convertir JSON a XML */
    ERROR /* Argumento inválido */
} tOperacion;

typedef enum {
    SUCCESS, /* Éxito */
    MEM, /* Error de memoria */
    ORIG, /* Error en la ruta de origen */
    DEST /* Error en la ruta de destino */
} tError;

/* Funcion que se encarga de identificar la operación a realizar
 * @param  arg  argumento ingresado por el usuario
 * @param  orden  identificador de la operación
 * @pos devuelve -1 si el argumento es incorrecto
 */
void Inicializar(char *arg, tOperacion *Ope)
{
    if ((strcmp(arg,"-xml2json") == 0))
        *Ope = XML2JSON;
    else
    {
        if ((strcmp(arg,"-json2xml") == 0))
            *Ope = JSON2XML;
        else
            *Ope = ERROR;
    }
}

/* Funcion que se encarga de mostrar un mensaje por pantalla de acuerdo al error */
void MostrarMensajeError(tError Code)
{
    switch (Code)
    {
        case(MEM):
        {
            printf(OUT_OF_MEM);
            break;
        }
        case(ORIG):
        {
            printf(RUTA_ORIGEN);
            break;
        }
        case(DEST):
        {
            printf(RUTA_DESTINO);
            break;
        }
        case(SUCCESS): break;
    }
}

/* Funcion que se encarga de inicializar una cadena de caracteres char *
 * @param  s   cadena de caracteres a inicializar
 * @pos    devuelve MEM - error de memoria - si no puede asignar memoria
 *         devuelve SUCCESS e inicializa la cadena si puede asignar memoria
 */
tError InicializarString(char* s)
{
    if ((s = (char*)malloc(MAX_LONG_RUTA)) == NULL) /* No pude asignar memoria */
        return MEM;
    memset(s,C_NULO,1); /* Inicializo la cadena */
    return SUCCESS;
}

int main(int argc, char *argv[])
{
    TDAConvertidor* TC = NULL;
    tOperacion Ope = VACIO;
    tError Code = SUCCESS;
    char* RutaOrigen;
    char* RutaDestino;
    int i = 1;
    if (CrearTC(TC)!=0)
        Code = MEM;
    Code = InicializarString(RutaOrigen);
    Code = InicializarString(RutaDestino);
    if (Code == SUCCESS) {
        if (argc == 1) /* El usuario no ingresó argumentos, además del nombre del programa */
            printf("Debe ingresar al menos un argumento seguido del nombre del programa.\n");
        else
        {
            do
            {
                Ope = VACIO;
                Inicializar(argv[i],&Ope);
                if (Ope != ERROR)
                {
                    if (i!=(argc-1))
                    { /* No es el último argumento --> puedo obtener la ruta de origen */
                        memcpy(RutaOrigen,argv[++i],MAX_LONG_RUTA);
                        if (i!=(argc-1))
                        { /* No es el último argumento --> puedo obtener la ruta de destino */
                            memcpy(RutaDestino,argv[++i],MAX_LONG_RUTA);
                        }
                        else /* Es el último argumento y no tengo la ruta de destino */
                            Code = DEST;
                    }
                    else /* Es el último argumento y no tengo la ruta de origen */
                        Code = ORIG;
                    switch (Ope)
                    {
                        case (XML2JSON):
                        {
                            Code = xml2json(TC,RutaOrigen,RutaDestino);
                            break;
                        }
                        case (JSON2XML):
                        {
                            Code = json2xml(TC,RutaOrigen,RutaDestino);
                            break;
                        }
                        default: break;
                    }
                }
            } while ((++i < argc) && (Code==SUCCESS));
        }
        DestruirTC(TC);
    }
    if (Code != SUCCESS)
        MostrarMensajeError(Code);
    return EXIT_SUCCESS;
}
