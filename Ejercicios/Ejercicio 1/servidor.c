#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "ABB.h"
#include "utils.h"

#define INIT 0
#define GET 1
#define SET 2
#define MODIFY 3
#define DELETE 4
#define EXIST 5

#define MAX_BUFFER 10           /* tamaño del buffer */
#define DATOS_A_PRODUCIR 100000 /* datos a producir */

void data_struct_test1()
{
    struct Tree *tree;
    init_tree(&tree);

    int key1 = 100;
    int key2 = 50;
    int key3 = 200;
    int key4 = 75;
    int key5 = 25;
    int key6 = 15;
    int key7 = 35;
    int key8 = 175;
    char cadena[MAX_LENGTH] = "abcdef";
    int N = 2;
    double vector[2] = {1.5, 1.5};
    print_tree(tree, 0);
    post_node(tree, key1, cadena, N, vector);
    print_tree(tree, 0);
    post_node(tree, key1, cadena, N, vector);
    post_node(tree, key2, cadena, N, vector);
    post_node(tree, key3, cadena, N, vector);
    post_node(tree, key4, cadena, N, vector);
    post_node(tree, key5, cadena, N, vector);
    post_node(tree, key6, cadena, N, vector);
    post_node(tree, key7, cadena, N, vector);
    post_node(tree, key8, cadena, N, vector);
    post_node(tree, key5, cadena, N, vector);
    print_tree(tree, 1);
}

void data_struct_test2()
{
    struct Tree *tree;
    init_tree(&tree);

    int key1 = 100;
    char cadena1[MAX_LENGTH] = "abcd";
    int N1 = 2;
    double vector1[2] = {1.1, 1.2};

    int key2 = 50;
    char cadena2[MAX_LENGTH] = "efgh";
    int N2 = 3;
    double vector2[3] = {1.3, 1.4, 1.5};

    int key3 = 200;
    char cadena3[MAX_LENGTH] = "ijkl";
    int N3 = 4;
    double vector3[4] = {1.6, 1.7, 1.8, 1.9};

    char cadena_dest[MAX_LENGTH];
    int N_dest;
    double *vector_dest = malloc(32 * sizeof(double));
    post_node(tree, key1, cadena1, N1, vector1);
    post_node(tree, key2, cadena2, N2, vector2);
    post_node(tree, key3, cadena3, N3, vector3);
    print_tree(tree, 1);
    get_node(tree, key3, cadena_dest, &N_dest, vector_dest);
    printf("Resultado: %s, %d, ", cadena_dest, N_dest);
    print_double_array(vector_dest, N_dest);
}

void data_struct_test3()
{
    struct Tree *tree;
    init_tree(&tree);

    int key1 = 100;
    char cadena1[MAX_LENGTH] = "abcd";
    int N1 = 2;
    double vector1[2] = {1.1, 1.2};

    int key2 = 50;
    char cadena2[MAX_LENGTH] = "efgh";
    int N2 = 3;
    double vector2[3] = {1.3, 1.4, 1.5};

    int key3 = 200;
    char cadena3[MAX_LENGTH] = "ijkl";
    int N3 = 4;
    double vector3[4] = {1.6, 1.7, 1.8, 1.9};

    char nueva_cadena[MAX_LENGTH] = "mnño";
    int nueva_N = 5;
    double nuevo_vector[5] = {2.0, 2.1, 2.2, 2.3, 2.4};
    post_node(tree, key1, cadena1, N1, vector1);
    post_node(tree, key2, cadena2, N2, vector2);
    post_node(tree, key3, cadena3, N3, vector3);
    print_tree(tree, 1);
    patch_node(tree, key1, nueva_cadena, nueva_N, nuevo_vector);
    print_tree(tree, 1);
}

void data_struct_test4()
{
    struct Tree *tree;
    init_tree(&tree);

    int key1 = 100;
    char cadena1[MAX_LENGTH] = "abcd";
    int N1 = 2;
    double vector1[2] = {1.1, 1.2};

    int key2 = 50;
    char cadena2[MAX_LENGTH] = "efgh";
    int N2 = 3;
    double vector2[3] = {1.3, 1.4, 1.5};

    int key3 = 200;
    char cadena3[MAX_LENGTH] = "ijkl";
    int N3 = 4;
    double vector3[4] = {1.6, 1.7, 1.8, 1.9};

    int key4 = 150;
    char cadena4[MAX_LENGTH] = "ijkl";
    int N4 = 4;
    double vector4[4] = {1.6, 1.7, 1.8, 1.9};

    int key5 = 101;
    char cadena5[MAX_LENGTH] = "ijkl";
    int N5 = 4;
    double vector5[4] = {1.6, 1.7, 1.8, 1.9};

    post_node(tree, key1, cadena1, N1, vector1);
    post_node(tree, key2, cadena2, N2, vector2);
    post_node(tree, key3, cadena3, N3, vector3);
    post_node(tree, key4, cadena4, N4, vector4);
    post_node(tree, key5, cadena5, N5, vector5);
    print_tree(tree, 1);
    delete_node(tree, key1);
    print_tree(tree, 1);
}

void data_struct_test5()
{
    struct Tree *tree;
    init_tree(&tree);

    int key1 = 100;
    char cadena1[MAX_LENGTH] = "abcd";
    int N1 = 2;
    double vector1[2] = {1.1, 1.2};

    int key2 = 50;
    char cadena2[MAX_LENGTH] = "efgh";
    int N2 = 3;
    double vector2[3] = {1.3, 1.4, 1.5};

    int key3 = 200;
    char cadena3[MAX_LENGTH] = "ijkl";
    int N3 = 4;
    double vector3[4] = {1.6, 1.7, 1.8, 1.9};

    post_node(tree, key1, cadena1, N1, vector1);
    post_node(tree, key2, cadena2, N2, vector2);
    post_node(tree, key3, cadena3, N3, vector3);
    print_tree(tree, 1);
    if (head_node(tree, 201) == 1)
    {
        printf("Node exists\n");
    }
    else
    {
        printf("Node doesn't exist\n");
    }
}

void init()
{
    printf("Init service\n");
}

void get_tuple(char *input)
{
    printf("Get tuple service + %s\n", input);
}

void set_tuple(char *input)
{
    printf("Set tuple service + %s\n", input);
}

void modify_tuple(char *input)
{
    printf("Modify tuple service + %s\n", input);
}

void delete_tuple(char *input)
{
    printf("Delete tuple service + %s\n", input);
}

void exist_tuple(char *input)
{
    printf("Exist tuple service + %s\n", input);
}

int main(int argc, char **argv)
{

    mqd_t mq; /* cola de mensajes donde dejar los    datos
  producidos y extraer los datos a consumir */
    struct mq_attr attr;

    attr.mq_maxmsg = MAX_BUFFER;
    attr.mq_msgsize = sizeof(char);
    mq = mq_open("/tuple_sv_queue", O_CREAT | O_RDWR, 0700, &attr);
    if (mq == -1)
    {
        perror("mq_open");
        exit(-1);
    }

    char *dato = malloc(sizeof(char));

    for (;;)
    {
        /* recibir dato */
        printf("Esperando un dato\n");
        if (mq_receive(mq, dato, sizeof(char), 0) == -1)
        {
            perror("mq_receive");
            mq_close(mq);
            exit(1);
        }
        /* Consumir el dato */
        char *end;

        switch (strtol(dato, &end, 10))
        {
        case INIT:
            init();
            break;

        case GET:
            get_tuple(dato);
            break;

        case SET:
            set_tuple(dato);
            break;

        case MODIFY:
            modify_tuple(dato);
            break;

        case DELETE:
            delete_tuple(dato);
            break;

        case EXIST:
            exist_tuple(dato);
            break;

        default:
            printf("El dato consumido es: %s\n", dato);

            if (*end == '\0')
            {
                printf("El código de operación debe estar entre 0 y 5\n");
            }
            else
            {
                printf("Error al registrar el número de operación\n");
            }

            break;
        }
    } /* end for */

    mq_close(mq);

    return (0);
}