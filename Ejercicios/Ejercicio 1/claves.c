#include "claves.h"
#include "mensaje.h"

#include <stdlib.h>
#include <stdio.h>
#include <mqueue.h>
#include <errno.h>
#include <string.h>

#define QUEUE_NAME "/tuple_sv_queue"
#define MAX_MSG_SIZE 256
#define MAX_LENGTH 255

#define INIT "0"
#define GET "1"
#define SET "2"
#define MODIFY "3"
#define DELETE "4"
#define EXIST "5"

mqd_t mq;

int init()
{
    // Inicializa el servicio de tuplas,
    // se eliminarán todas las tuplas que
    // existiesen anteriormente

    // Abrir la cola de mensajes
    mq = mq_open(QUEUE_NAME, O_RDWR);
    if (mq == -1)
    {
        perror("mq_open");
        return -1;
    }

    // char *dato = malloc(sizeof(char)); // Asignar memoria para dato
    // if (dato == NULL)
    // {
    //     perror("malloc");
    //     exit(-1);
    // }

    /* producir dato */
    struct Mensaje msg;

    strcpy(msg.op, INIT);

    printf("Enviando: %s\n", msg.op);

    if (mq_send(mq, (const char *)&msg, sizeof(msg), 0) < 0)
    {
        perror("mq_send");
        mq_close(mq);
        exit(1);
    }

    // Cerrar la cola de mensajes
    mq_close(mq);
    return 0;
}

int set_value(int key, char *value1, int N_value2, double *V_value2)
{
    // Inserta la tupla definida por los
    // argumentos

    // Abrir la cola de mensajes
    mq = mq_open(QUEUE_NAME, O_RDWR);
    if (mq == -1)
    {
        perror("mq_open");
        return -1;
    }

    /* producir dato */
    struct Mensaje msg;

    strcpy(msg.op, SET);
    msg.key = key;
    strcpy(msg.cadena, value1);
    msg.N = N_value2;
    msg.vector = V_value2;

    printf("Enviando: %s\n", msg.op);

    if (mq_send(mq, (const char *)&msg, sizeof(msg), 0) < 0)
    {
        perror("mq_send");
        mq_close(mq);
        exit(1);
    }

    // Cerrar la cola de mensajes
    mq_close(mq);

    return 0;
}

int get_value(int key, char *value1, int *N_value2, double *V_value2)
{
    // Deuvelve la tupla asociada a key
    // en los punteros pasados por argumentos
    return 0;
}

int modify_value(int key, char *value1, int N_value2, double *V_value2)
{
    return 0;
}

int delete_key(int key)
{
    return 0;
}

int exist_key(int key)
{
    return 0;
}