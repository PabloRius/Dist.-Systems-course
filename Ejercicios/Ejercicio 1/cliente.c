#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include "claves.h"

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

    init();

    set_value(key1, cadena1, N2, vector1);
    sleep(2);
    set_value(key1, cadena1, N2, vector1);
    return (0);
}
