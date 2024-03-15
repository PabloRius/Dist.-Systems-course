#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include "claves.h"
#include "utils.h"

#define MAX_LENGTH 255

int main()
{

    int key1 = 100;
    char cadena1[MAX_LENGTH] = "abcd";
    int N1 = 2;
    double vector1[2] = {1.1, 1.2};

    int key2 = 50;
    char cadena2[MAX_LENGTH] = "efgh";
    int N2 = 3;
    double vector2[3] = {1.3, 1.4, 1.5};

    char cadena3[MAX_LENGTH];
    int N3;
    double vector3[32];

    char cadena4[MAX_LENGTH];
    int N4;
    double vector4[32];

    init();

    set_value(key1, cadena1, N1, vector1);
    // sleep(2);
    set_value(key2, cadena2, N2, vector2);
    if (get_value(key1, cadena3, &N3, vector3) >= 0)
    {
        printf("%s, %d, ", cadena3, N3);
        print_double_array(vector3, N3);
    }
    if (get_value(key2, cadena4, &N4, vector4) >= 0)
    {
        printf("%s, %d, ", cadena4, N4);
        print_double_array(vector4, N4);
    }

    return (0);
}
