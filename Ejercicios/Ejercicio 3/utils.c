#include "utils.h"
#include "comm.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_double_array(double *array, int size)
{
    printf("[");
    for (int i = 0; i < size; i++)
    {
        printf("%.2f", array[i]);
        if (i < size - 1)
        {
            printf(", ");
        }
    }
    printf("]\n");
}

void datoc(double *array, int size, char *output)
{
    char *serializer = "|";
    output[0] = '\0';
    for (int i = 0; i < size; i++)
    {
        if (i != size - 1)
        {
            char current[100];
            sprintf(current, "%f%s", array[i], serializer);
            strcat(output, current);
        }
        else
        {
            char current[100];
            sprintf(current, "%f", array[i]);
            strcat(output, current);
        }
    }
}

void ctoda(double *array, int size, char *input)
{
    const char *delimiter = "|";
    char *token;
    int index = 0;

    token = strtok(input, delimiter);

    while (token != NULL && index < size)
    {
        array[index++] = atof(token);
        token = strtok(NULL, delimiter);
    }
}

int conn_socket(char *HOST, int PORT)
{
    int sd = clientSocket(HOST, PORT);
    if (sd < 0)
    {
        printf("Error en clientSocket with %s:%d\n", HOST, PORT);
        return -1;
    }
    return sd;
}