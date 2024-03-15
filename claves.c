#include "claves.h"
#include "mensaje.h"

#include <stdlib.h>
#include <stdio.h>
#include <mqueue.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

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

    /* producir dato */
    struct Mensaje msg;

    strcpy(msg.op, INIT);
    char queue_name[MAX_LENGTH];
    sprintf(queue_name, "/Cola-%d", getpid());
    strcpy(msg.queue, queue_name);

    mqd_t mq_rcv;
    struct mq_attr attr;

    attr.mq_maxmsg = 1;
    attr.mq_msgsize = sizeof(struct Respuesta);
    mq_rcv = mq_open(queue_name, O_CREAT | O_RDONLY, 0700, &attr);
    if (mq_rcv == -1)
    {
        perror("mq_open");
        exit(-1);
    }

    printf("Enviando: %s, %s\n", msg.op, msg.queue);

    if (mq_send(mq, (const char *)&msg, sizeof(msg), 0) < 0)
    {
        perror("mq_send");
        mq_close(mq);
        exit(1);
    }
    struct Respuesta res;

    printf("Esperando un dato\n");
    if (mq_receive(mq_rcv, (char *)&res, sizeof(res), 0) == -1)
    {
        perror("mq_receive");
        mq_close(mq_rcv);
        exit(1);
    }
    printf("Respuesta: %d\n", res.codigo);

    // Cerrar la cola de mensajes
    mq_close(mq);
    mq_close(mq_rcv);
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

int get_value(int key, char *value1, int *N_value2, double *V_value2) {
    struct Mensaje msg;
    strcpy(msg.op, GET);
    msg.key = key;

    // Abrir la cola de mensajes para envío
    mqd_t mq = mq_open(QUEUE_NAME, O_RDWR);
    if (mq == -1) {
        perror("mq_open");
        return -1;
    }

    // Enviar solicitud al servidor
    if (mq_send(mq, (const char *)&msg, sizeof(msg), 0) < 0) {
        perror("mq_send");
        mq_close(mq);
        return -1;
    }

    // Esperar y recibir respuesta
    struct Respuesta res;
    if (mq_receive(mq, (char *)&res, sizeof(res), NULL) == -1) {
        perror("mq_receive");
        mq_close(mq);
        return -1;
    }

    // Procesar la respuesta y llenar los valores de retorno
    if (res.codigo == 0) {
        strcpy(value1, res.value1);
        *N_value2 = res.N;
        memcpy(V_value2, res.V_value2, (*N_value2) * sizeof(double));
    } else {
        // Manejo de error o clave no encontrada
        mq_close(mq);
        return -1;
    }

    mq_close(mq);
    return 0;
}


int modify_value(int key, char *value1, int N_value2, double *V_value2)
{   // Preparar mensaje
    // struct Mensaje msg;
    strcpy(msg.op, MODIFY);
    msg.key = key;
    strcpy(msg.value1, value1);
    msg.N = N_value2;
    memcpy(msg.vector, V_value2, N_value2 * sizeof(double));

    // Abrir la cola de mensajes
    mqd_t mq = mq_open(QUEUE_NAME, O_RDWR);
    if (mq == -1) {
        perror("mq_open");
        return -1;
    }

    // Enviar solicitud de modificación al servidor
    if (mq_send(mq, (const char *)&msg, sizeof(msg), 0) < 0) {
        perror("mq_send");
        mq_close(mq);
        return -1;
    }

    // Esperar y recibir respuesta
    struct Respuesta res;
    if (mq_receive(mq, (char *)&res, sizeof(res), NULL) == -1) {
        perror("mq_receive");
        mq_close(mq);
        return -1;
    }

    mq_close(mq);
    return 0;
}

int delete_key(int key)
{// Preparar mensaje
    struct Mensaje msg;
    strcpy(msg.op, DELETE);
    msg.key = key;

    // Abrir la cola de mensajes
    mqd_t mq = mq_open(QUEUE_NAME, O_RDWR);
    if (mq == -1) {
        perror("mq_open");
        return -1;
    }

    // Enviar solicitud de eliminación al servidor
    if (mq_send(mq, (const char *)&msg, sizeof(msg), 0) < 0) {
        perror("mq_send");
        mq_close(mq);
        return -1;
    }

    // Esperar y recibir respuesta
    struct Respuesta res;
    if (mq_receive(mq, (char *)&res, sizeof(res), NULL) == -1) {
        perror("mq_receive");
        mq_close(mq);
        return -1;
    }

    mq_close(mq);
    return 0;
}

int exist_key(int key)
{
    // Preparar mensaje
    struct Mensaje msg;
    strcpy(msg.op, EXIST);
    msg.key = key;

    // Abrir la cola de mensajes
    mqd_t mq = mq_open(QUEUE_NAME, O_RDWR);
    if (mq == -1) {
        perror("mq_open");
        return -1;
    }

    // Enviar solicitud de existencia al servidor
    if (mq_send(mq, (const char *)&msg, sizeof(msg), 0) < 0) {
        perror("mq_send");
        mq_close(mq);
        return -1;
    }

    // Esperar y recibir respuesta del servidor
    struct Respuesta res;
    if (mq_receive(mq, (char *)&res, sizeof(res), NULL) == -1) {
        perror("mq_receive");
        mq_close(mq);
        return -1;
    }

    mq_close(mq);
    // Interpretar respuesta
    if (res.codigo == 0) { // Asumiendo código 0 para "no existe"
        return 0;
    } else if (res.codigo == 1) { // Asumiendo código 1 para "existe"
        return 1;
    }

    return -1; // Error
}
