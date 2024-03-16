#include "utils.h"

#include <stdio.h>

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