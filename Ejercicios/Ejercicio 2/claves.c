#include "claves.h"
#include "utils.h"
#include "comm.h"

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern char *HOST;
extern int PORT;

#define QUEUE_NAME "/tuple_sv_queue"
#define MAX_MSG_SIZE 256
#define MAX_LENGTH 255

#define INIT 0
#define GET 1
#define SET 2
#define MODIFY 3
#define DELETE 4
#define EXIST 5

int init()
{
    int sd = conn_socket(HOST, PORT);
    int ret;
    char op;
    int32_t res;

    op = INIT;
    ret = sendMessage(sd, (char *)&op, sizeof(char));
    if (ret == -1)
    {
        printf("Error envío op\n");
        return -1;
    }
    printf("Resperando respuests\n");
    ret = recvMessage(sd, (char *)&res, sizeof(int32_t));
    if (ret == -1)
    {
        printf("Error en recepción\n");
        return -1;
    }
    res = ntohl(res); // unmarshalling: *r <- network to host long(res)
    printf("Resultado del init: %d\n", res);

    closeSocket(sd);
    return 0;
}

int set_value(int key, char *value1, int N_value2, double *V_value2)
{
    int sd = conn_socket(HOST, PORT);
    int ret;
    char op;
    int32_t res;

    op = SET;

    ret = sendMessage(sd, (char *)&op, sizeof(char));
    if (ret == -1)
    {
        printf("Error envío op\n");
        return -1;
    }
    int32_t key32 = htonl(key);
    ret = sendMessage(sd, (char *)&key32, sizeof(int32_t));
    if (ret == -1)
    {
        printf("Error envío key\n");
        return -1;
    }
    ret = sendMessage(sd, (char *)value1, 255 * sizeof(char));
    if (ret == -1)
    {
        printf("Error envío de cadena\n");
        return -1;
    }
    int32_t N = htonl(N_value2);
    ret = sendMessage(sd, (char *)&N, sizeof(int32_t));
    if (ret == -1)
    {
        printf("Error envío N\n");
        return -1;
    }
    char vector_to_array[102 * N_value2];
    datoc(V_value2, N_value2, vector_to_array);
    ret = sendMessage(sd, (char *)vector_to_array, 102 * N_value2 * sizeof(char));
    if (ret == -1)
    {
        printf("Error envío de vector de doubles\n");
        return -1;
    }

    printf("Resperando respuesta\n");
    ret = recvMessage(sd, (char *)&res, sizeof(int32_t));
    if (ret == -1)
    {
        printf("Error en recepción\n");
        return -1;
    }
    res = ntohl(res);
    printf("Resultado del set: %d\n", res);

    closeSocket(sd);
    return 0;
}

int get_value(int key, char *value1, int *N_value2, double *V_value2)
{
    int sd = conn_socket(HOST, PORT);
    int ret;
    char op;
    int32_t res;

    op = GET;

    ret = sendMessage(sd, (char *)&op, sizeof(char));
    if (ret == -1)
    {
        printf("Error envío op\n");
        return -1;
    }
    int32_t key32 = htonl(key);
    ret = sendMessage(sd, (char *)&key32, sizeof(int32_t));
    if (ret == -1)
    {
        printf("Error envío key\n");
        return -1;
    }
    printf("Resperando respuesta\n");
    ret = recvMessage(sd, (char *)&res, sizeof(int32_t));
    if (ret == -1)
    {
        printf("Error en recepción\n");
        return -1;
    }
    res = ntohl(res);
    printf("Resultado del get: %d\n", res);
    if (res >= 0)
    {
        char cadena[255];
        int N;
        printf("Resperando el resto de argumentos\n");
        ret = recvMessage(sd, (char *)cadena, 255 * sizeof(char));
        if (ret < 0)
        {
            printf("Error en la recepción de la cadena\n");

            return -1;
        }
        ret = recvMessage(sd, (char *)&N, sizeof(int32_t));
        if (ret < 0)
        {
            printf("Error en la recepción de N\n");

            return -1;
        }
        N = ntohl(N);
        char char_double_array[102 * N];
        ret = recvMessage(sd, (char *)char_double_array, 102 * N * sizeof(char));
        if (ret < 0)
        {
            printf("Error en la recepción del vector\n");

            return -1;
        }
        double vector[N];
        ctoda(vector, N, char_double_array);

        printf("cadena: %s, N:%d, vector: ", cadena, N);
        print_double_array(vector, N);

        strcpy(value1, cadena);
        *N_value2 = N;
        memcpy(V_value2, vector, N * sizeof(double));
    }

    closeSocket(sd);
    return 0;
}

int modify_value(int key, char *value1, int N_value2, double *V_value2)
{
    int sd = conn_socket(HOST, PORT);
    int ret;
    char op;
    int32_t res;

    op = MODIFY;

    ret = sendMessage(sd, (char *)&op, sizeof(char));
    if (ret == -1)
    {
        printf("Error envío op\n");
        return -1;
    }
    int32_t key32 = htonl(key);
    ret = sendMessage(sd, (char *)&key32, sizeof(int32_t));
    if (ret == -1)
    {
        printf("Error envío key\n");
        return -1;
    }
    ret = sendMessage(sd, (char *)value1, 255 * sizeof(char));
    if (ret == -1)
    {
        printf("Error envío de cadena\n");
        return -1;
    }
    int32_t N = htonl(N_value2);
    ret = sendMessage(sd, (char *)&N, sizeof(int32_t));
    if (ret == -1)
    {
        printf("Error envío N\n");
        return -1;
    }
    char vector_to_array[102 * N_value2];
    datoc(V_value2, N_value2, vector_to_array);
    ret = sendMessage(sd, (char *)vector_to_array, 102 * N_value2 * sizeof(char));
    if (ret == -1)
    {
        printf("Error envío de vector de doubles\n");
        return -1;
    }

    printf("Resperando respuesta\n");
    ret = recvMessage(sd, (char *)&res, sizeof(int32_t));
    if (ret == -1)
    {
        printf("Error en recepción\n");
        return -1;
    }
    res = ntohl(res);
    printf("Resultado del modify: %d\n", res);

    closeSocket(sd);
    return 0;
}

int delete_key(int key)
{
    int sd = conn_socket(HOST, PORT);
    int ret;
    char op;
    int32_t res;

    op = DELETE;

    ret = sendMessage(sd, (char *)&op, sizeof(char));
    if (ret == -1)
    {
        printf("Error envío op\n");
        return -1;
    }
    int32_t key32 = htonl(key);
    ret = sendMessage(sd, (char *)&key32, sizeof(int32_t));
    if (ret == -1)
    {
        printf("Error envío key\n");
        return -1;
    }

    printf("Resperando respuesta\n");
    ret = recvMessage(sd, (char *)&res, sizeof(int32_t));
    if (ret == -1)
    {
        printf("Error en recepción\n");
        return -1;
    }
    res = ntohl(res);
    printf("Resultado del delete: %d\n", res);

    closeSocket(sd);
    return 0;
}

int exist(int key)
{
    int sd = conn_socket(HOST, PORT);
    int ret;
    char op;
    int32_t res;

    op = EXIST;

    ret = sendMessage(sd, (char *)&op, sizeof(char));
    if (ret == -1)
    {
        printf("Error envío op\n");
        return -1;
    }
    int32_t key32 = htonl(key);
    ret = sendMessage(sd, (char *)&key32, sizeof(int32_t));
    if (ret == -1)
    {
        printf("Error envío key\n");
        return -1;
    }

    printf("Resperando respuesta\n");
    ret = recvMessage(sd, (char *)&res, sizeof(int32_t));
    if (ret == -1)
    {
        printf("Error en recepción\n");
        return -1;
    }
    res = ntohl(res);
    printf("Resultado del exist: %d\n", res);

    closeSocket(sd);
    return 0;
}
