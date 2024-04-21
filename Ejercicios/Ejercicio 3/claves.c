#include "tuplas.h"
#include "claves.h"
#include "utils.h"
#include "comm.h"

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#define INIT 0
#define GET 1
#define SET 2
#define MODIFY 3
#define DELETE 4
#define EXIST 5

int init()
{
    CLIENT *cl;
    char *host;
    int *result;

    cl = clnt_create(host, TUPLE_PROG, TUPLE_VERS, "tcp");
    if (cl == NULL) {
        clnt_pcreateerror(host);
        exit(1);
    }

    result = init_1(cl);
    if (result == NULL) {
        clnt_perror(cl, "call failed");
    }

    clnt_destroy(cl);
    return *result;
}


int set_value(int key, char *value1, int N_value2, double *V_value2)
{
    CLIENT *cl;
    char *host;
    int *result;
    set_value_args args; // Estructura definida en tuplas.h

    // Asignar los valores a la estructura de argumentos
    args.key = key;
    args.value1 = value1;
    args.N_value2 = N_value2;
    memcpy(args.V_value2, V_value2, N_value2 * sizeof(double));

    // Crear el cliente RPC
    cl = clnt_create(host, TUPLE_PROG, TUPLE_VERS, "tcp");
    if (cl == NULL) {
        clnt_pcreateerror(host);
        exit(1);
    }

    // Llamar al procedimiento remoto
    result = set_value_1(&args, cl);
    if (result == NULL) {
        clnt_perror(cl, "call failed");
        clnt_destroy(cl);
        return -1;
    }

    // Limpiar y retornar el resultado
    clnt_destroy(cl);
    return *result;
}

int get_value(int key, char *value1, int *N_value2, double *V_value2)
{
    CLIENT *cl;
    char *host;
    get_value_args args;
    get_value_result *result;

    args.key = key;

    // Crear el cliente RPC
    cl = clnt_create(host, TUPLE_PROG, TUPLE_VERS, "tcp");
    if (cl == NULL) {
        clnt_pcreateerror(host);
        exit(1);
    }

    // Llamar al procedimiento remoto
    result = get_value_1(&args, cl);
    if (result == NULL) {
        clnt_perror(cl, "call failed");
        clnt_destroy(cl);
        return -1;
    }

    if (result->status == 0) {
        strcpy(value1, result->value1);
        *N_value2 = result->N_value2;
        memcpy(V_value2, result->V_value2, result->N_value2 * sizeof(double));
    }

    // Limpiar y retornar el resultado del status
    clnt_destroy(cl);
    return result->status;
}

int modify_value(int key, char *value1, int N_value2, double *V_value2)
{
    CLIENT *cl;
    char *host ;
    modify_value_args args;
    int *result;

    // Configurar los argumentos
    args.key = key;
    args.value1 = value1;
    args.N_value2 = N_value2;
    memcpy(args.V_value2, V_value2, N_value2 * sizeof(double));

    // Crear el cliente RPC
    cl = clnt_create(host, TUPLE_PROG, TUPLE_VERS, "tcp");
    if (cl == NULL) {
        clnt_pcreateerror(host);
        exit(1);
    }

    // Llamar al procedimiento remoto
    result = modify_value_1(&args, cl);
    if (result == NULL) {
        clnt_perror(cl, "call failed");
        clnt_destroy(cl);
        return -1;
    }

    // Limpiar y retornar el resultado
    clnt_destroy(cl);
    return *result;
}

int delete_key(int key)
{
    CLIENT *cl;
    char *host;
    delete_key_args args;
    int *result;

    // Configurar los argumentos
    args.key = key;

    // Crear el cliente RPC
    cl = clnt_create(host, TUPLE_PROG, TUPLE_VERS, "tcp");
    if (cl == NULL) {
        clnt_pcreateerror(host);
        exit(1);
    }

    // Llamar al procedimiento remoto
    result = delete_key_1(&args, cl);
    if (result == NULL) {
        clnt_perror(cl, "call failed");
        clnt_destroy(cl);
        return -1;
    }

    // Limpiar y retornar el resultado
    clnt_destroy(cl);
    return *result;
}

int exist(int key)
{
    CLIENT *cl;
    char *host;
    exist_args args;
    int *result;

    // Configurar los argumentos
    args.key = key;

    // Crear el cliente RPC
    cl = clnt_create(host, TUPLE_PROG, TUPLE_VERS, "tcp");
    if (cl == NULL) {
        clnt_pcreateerror(host);
        exit(1);
    }

    // Llamar al procedimiento remoto
    result = exist_1(&args, cl);
    if (result == NULL) {
        clnt_perror(cl, "call failed");
        clnt_destroy(cl);
        return -1; // Retornar -1 en caso de error de llamada
    }

    int res = *result; // Guardar el resultado antes de destruir el cliente
    // Limpiar
    clnt_destroy(cl);

    return res; // Devolver el resultado de la operación
}























