#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "comm.h"
#include "utils.h"
#include "claves.h"

#define MAX_LENGTH 255

int main()
{
    int key1 = 100;
    int key2 = 50;

    char cadena1[MAX_LENGTH] = "abcd";
    int N1 = 2;
    double vector1[2] = {1.1, 1.2};

    char cadena2[MAX_LENGTH] = "efgh";
    int N2 = 3;
    double vector2[3] = {1.3, 1.4, 1.5};

    char cadena4[MAX_LENGTH] = "Pablo Garcia";
    int N4 = 5;
    double vector4[5] = {2.1, 2.2, 2.3, 2.4, 2.5};

    char cadena5[MAX_LENGTH] = "";
    int N5 = 4;
    double vector5[5] = {3.1, 3.2, 3.3, 3.4};

    char cadena7[MAX_LENGTH] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Maecenas eu purus iaculis, interdum";
    int N7 = 10;
    double vector7[10] = {1.8, 1.9, 2.0, 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7};

    // iniciamos el fichero de claves
    printf("TEST 1\n");
    init();

    // set values
    printf("TEST 2\n");
    set_value(key1, cadena1, N1, vector1);
    printf("TEST 3\n");
    set_value(key2, cadena2, N2, vector2);

    // Comprobamos que se han introducido las tuplas
    printf("TEST 4\n");
    char cadena3[MAX_LENGTH];
    int N3;
    double vector3[32];
    if (get_value(key1, cadena3, &N3, vector3) >= 0)
    {
        printf("--RESULTADO--: Cadena: %s, N: %d, vector: ", cadena3, N3);
        print_double_array(vector3, N3);
    }
    printf("TEST 5\n");
    if (get_value(key2, cadena3, &N3, vector3) >= 0)
    {
        printf("--RESULTADO--: Cadena: %s, N: %d, vector: ", cadena3, N3);
        print_double_array(vector3, N3);
    }
    // Un get inexistente
    printf("TEST 6\n");
    if (get_value(1234, cadena3, &N3, vector3) >= 0)
    {
        printf("--RESULTADO--: Cadena: %s, N: %d, vector: ", cadena3, N3);
        print_double_array(vector3, N3);
    }

    // Modificamos valores
    printf("TEST 7\n");
    if (modify_value(key1, cadena4, N4, vector4) >= 0)
    {
        if (get_value(key1, cadena3, &N3, vector3) >= 0)
        {
            printf("--RESULTADO--: Cadena: %s, N: %d, vector: ", cadena3, N3);
            print_double_array(vector3, N3);
        }
    }
    // Modify con clave inexistente
    printf("TEST 8\n");
    modify_value(1234, cadena4, N4, vector4);

    // Modify con cadena vacía
    printf("TEST 9\n");
    if (modify_value(key2, cadena5, N5, vector5) >= 0)
    {
        if (get_value(key2, cadena3, &N3, vector3) >= 0)
        {
            printf("--RESULTADO--: Cadena: %s, N: %d, vector: ", cadena3, N3);
            print_double_array(vector3, N3);
        }
    }

    // Introducimos varias tuplas seguidas
    printf("TEST 11\n");
    for (int i = 101; i < 150; i++)
    {
        set_value(i, cadena7, N7, vector7);
    }
    // Comprobamos que se han metido extrayendo una de ellas
    printf("TEST 12\n");
    if (get_value(127, cadena3, &N3, vector3) >= 0)
    {
        printf("--RESULTADO--: Cadena: %s, N: %d, vector: ", cadena3, N3);
        print_double_array(vector3, N3);
    }

    // Eliminamos una de las claves que introdujimos
    printf("TEST 13\n");
    delete_key(127);
    // Comprobamos que se ha eliminado extrayéndola
    printf("TEST 14\n");
    if (get_value(127, cadena3, &N3, vector3) >= 0)
    {
        printf("--RESULTADO--: Cadena: %s, N: %d, vector: ", cadena3, N3);
        print_double_array(vector3, N3);
    }

    // Probamos el exist con una clave existente y otra errónea
    printf("TEST 16\n");
    exist(key1);
    printf("TEST 17\n");
    exist(1000000);

    // exist(5);
    // exist(0);

    // // set values cadena longitud maxima y vector tamaño maximo
    // set_value(key5, cadena11, N11, vector11);
    // set_value(key6, cadena12, N12, vector12);

    return (0);
}
