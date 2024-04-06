#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "comm.h"
#include "utils.h"
#include "claves.h"

char *HOST;
int PORT;

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("Uso: %s <dirección servidor> <puerto servidor>\n", argv[0]);
        printf("Ejemplo -> %s localhost 4200\n\n", argv[0]);
        return (0);
    }

    HOST = argv[1];
    PORT = atoi(argv[2]);

    char cadena[255] = "Hola soy Pablo";
    int N = 3;
    double vector[3] = {1.2, 1.3, 1.4};

    char cadena2[255] = "Hola yo no soy Pablo";
    int N2 = 5;
    double vector2[5] = {1.2, 1.3, 1.4, 1.5, 1.6};

    char cadena3[255] = "Hola yo puede que sea Pablo";
    int N3 = 6;
    double vector3[6] = {1.2, 1.3, 1.4, 1.5, 1.6, 1.7};

    char cadena_get[255];
    int NGet;
    double *vector_get;
    init();
    set_value(200, cadena, N, vector);
    get_value(200, cadena_get, &NGet, vector_get);
    set_value(300, cadena2, N2, vector2);
    get_value(200, cadena_get, &NGet, vector_get);
    get_value(300, cadena_get, &NGet, vector_get);
    modify_value(200, cadena3, N3, vector3);
    get_value(200, cadena_get, &NGet, vector_get);
    get_value(300, cadena_get, &NGet, vector_get);
    delete_key(200);
    exist(300);
    exist(200);
    return 0;
}