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

    // set values 1
    int key1 = 100;
    char cadena1[MAX_LENGTH] = "abcd";
    int N1 = 2;
    double vector1[2] = {1.1, 1.2};

    // set values 2
    int key2 = 50;
    char cadena2[MAX_LENGTH] = "efgh";
    int N2 = 3;
    double vector2[3] = {1.3, 1.4, 1.5};

    // get values1
    char cadena3[MAX_LENGTH];
    int N3;
    double vector3[32];

    // get values 2
    char cadena4[MAX_LENGTH];
    int N4;
    double vector4[32];

    // modify values 1
    char cadena5[MAX_LENGTH] = "jose antonio";
    int N5 = 5;
    double vector5[5] = {2.1, 2.2, 2.3, 2.4, 2.5};

    // modify values 2, error (hemos introducido un valor del tamaño del vector erroneo)
    char cadena6[MAX_LENGTH] = "jose antonio";
    int N6 = 2;
    double vector6[5] = {2.1, 2.2, 2.3, 2.4};

    // modify values 3, cadena vacia
    char cadena7[MAX_LENGTH] = "";
    int N7 = 4;
    double vector7[5] = {3.1, 3.2, 3.3, 3.4};

    // modify values 4, vector vacio
    char cadena8[MAX_LENGTH] = "pablo";
    int N8 = 0;
    double vector8[0] ={};

    // set values para delete key 1
    int key3 = 200;
    char cadena9[MAX_LENGTH] = "sdgbs";
    int N9 = 2;
    double vector9[2] = {1.8, 1.9};

    // set values para delete key 2
    int key4 = 250;
    char cadena10[MAX_LENGTH] = "srthsdfb";
    int N10 = 2;
    double vector10[2] = {2.8, 2.9};



    init();

    //set values
    set_value(key1, cadena1, N1, vector1);
    set_value(key2, cadena2, N2, vector2);


    //get_values
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

    // modify_values
    if (modify_value(key1, cadena5, N5, vector5) >= 0)
    {
        printf("%s, %d, ", cadena5, N5);
        print_double_array(vector5, N5);
    }

    // modify_values con valor N erroneo
    if (modify_value(key2, cadena6, N6, vector6) >= 0)
    {
        printf("%s, %d, ", cadena6, N6);
        print_double_array(vector6, N6);
    }

    // modify_values, cadena modificada para que este vacia
    if (modify_value(key2, cadena7, N7, vector7) >= 0)
    {
        printf("%s, %d, ", cadena7, N7);
        print_double_array(vector7, N7);
    }

    // modify_values, vector sin elementos
    if (modify_value(key1, cadena8, N8, vector8) >= 0)
    {
        printf("%s, %d, ", cadena8, N8);
        print_double_array(vector8, N8);
    }


    //set values para delete key
    set_value(key3, cadena9, N9, vector9);
    set_value(key4, cadena10, N10, vector10);

    // delete key normal
    delete_key(key3);
    delete_key(key4);

    // delete key con una key no guardada
    delete_key(5);

    // exist key
    exist(key1);
    exist(key2);

    exist(5);
    exist(0);






    return (0);
}
