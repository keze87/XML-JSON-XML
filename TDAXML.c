#include "TDAXML.h"

int xmlCargar(TDAXML *TDAXml, char *rutaXml)
{
    FILE *archivoxml;
    archivoxml=fopen(rutaXml, "r");
    if (archivoxml){
        TDAXml->xmlFile=archivoxml;
        printf("SE ABRIO!\n");




        fclose(archivoxml);

    }
    else
        fprintf(stderr,"La ruta %s no es valida\n", rutaXml);

        printf("CARGO XML\n");
	return 0;

}

int xmlGuardar(TDAXML *TDAXml, char *rutaXml)
{
    FILE *archivoxml;
    archivoxml=fopen(rutaXml, "w");
    if (archivoxml){
        printf("SE ABRIO!\n");




        fclose(archivoxml);

    }
    else
        fprintf(stderr,"La ruta %s no es valida\n", rutaXml);

    printf("GUARDO XML\n");
	return 0;

}
