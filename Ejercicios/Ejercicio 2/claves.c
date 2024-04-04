#include "claves.h"
#include "mensaje.h"
#include "utils.h"

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
    // Inicializar la colas de mensajes
    mqd_t mq_msg;
    mqd_t mq_res;

    // Definir la cola para la respuesta del servidor
    // Nombre
    char queue_name[MAX_LENGTH];
    sprintf(queue_name, "/Cola-%d", getpid());

    // Atributos
    struct mq_attr attr;
    attr.mq_maxmsg = 1;
    attr.mq_msgsize = sizeof(struct Respuesta);

    // Abrir las colas
    mq_res = mq_open(queue_name, O_CREAT | O_RDONLY, 0700, &attr);
    if (mq_res == -1)
    { // Controlar errores al abrir las colas
        perror("Error al crear la cola de respuesta");
        exit(1);
    }
    mq_msg = mq_open(QUEUE_NAME, O_WRONLY);
    if (mq_msg == -1)
    { // Controlar errores al abrir las colas
        mq_close(mq_res);
        mq_unlink(queue_name);
        perror("Error al abrir la cola del servidor");
        exit(1);
    }

    // Producir dato
    struct Mensaje msg;

    strcpy(msg.op, INIT);          // Clave de operación
    strcpy(msg.queue, queue_name); // Nombre de la cola de respuesta

    // Enviar mensaje producido
    printf("Enviando mensaje\n");

    if (mq_send(mq_msg, (const char *)&msg, sizeof(msg), 0) < 0)
    {
        perror("Error al mandar el mensaje");
        mq_close(mq_msg);
        mq_close(mq_res);
        mq_unlink(queue_name);
        exit(1);
    }

    // Leer respuesta del servidor
    struct Respuesta res;

    printf("Esperando respuesta del servidor\n");
    if (mq_receive(mq_res, (char *)&res, sizeof(res), 0) == -1)
    {
        perror("Error al recibir el mensaje");
        mq_close(mq_msg);
        mq_close(mq_res);
        mq_unlink(queue_name);
        exit(1);
    }
    printf("Respuesta: %d\n", res.codigo);

    // Cerrar las colas de mensajes
    mq_close(mq_msg);
    mq_close(mq_res);
    mq_unlink(queue_name);
    return res.codigo;
}

int set_value(int key, char *value1, int N_value2, double *V_value2)
{

    // Inicializar la colas de mensajes
    mqd_t mq_msg;
    mqd_t mq_res;

    // Definir la cola para la respuesta del servidor
    // Nombre
    char queue_name[MAX_LENGTH];
    sprintf(queue_name, "/Cola-%d", getpid());

    // Atributos
    struct mq_attr attr;
    attr.mq_maxmsg = 1;
    attr.mq_msgsize = sizeof(struct Respuesta);

    // Abrir las colas
    mq_res = mq_open(queue_name, O_CREAT | O_RDONLY, 0700, &attr);
    if (mq_res == -1)
    { // Controlar errores al abrir las colas
        perror("Error al crear la cola de respuesta");
        exit(1);
    }
    mq_msg = mq_open(QUEUE_NAME, O_WRONLY);
    if (mq_msg == -1)
    { // Controlar errores al abrir las colas
        mq_close(mq_res);
        mq_unlink(queue_name);
        perror("Error al abrir la cola del servidor");
        exit(1);
    }

    // Producir dato
    struct Mensaje msg;

    strcpy(msg.op, SET);                                     // Clave de operación
    msg.key = key;                                           // Key
    strcpy(msg.cadena, value1);                              // Cadena
    msg.N = N_value2;                                        // N
    memcpy(msg.vector, V_value2, N_value2 * sizeof(double)); // Vector de doubles
    strcpy(msg.queue, queue_name);                           // Nombre de la cola de respuesta

    // Enviar mensaje producido
    printf("Enviando mensaje\n");

    if (mq_send(mq_msg, (const char *)&msg, sizeof(msg), 0) < 0)
    {
        perror("Error al mandar el mensaje");
        mq_close(mq_msg);
        mq_close(mq_res);
        mq_unlink(queue_name);
        exit(1);
    }

    // Leer respuesta del servidor
    struct Respuesta res;

    printf("Esperando respuesta del servidor\n");
    if (mq_receive(mq_res, (char *)&res, sizeof(res), 0) == -1)
    {
        perror("Error al recibir el mensaje");
        mq_close(mq_msg);
        mq_close(mq_res);
        mq_unlink(queue_name);
        exit(1);
    }
    printf("Respuesta: %d\n", res.codigo);

    // Cerrar las colas de mensajes
    mq_close(mq_msg);
    mq_close(mq_res);
    mq_unlink(queue_name);
    return res.codigo;
}

int get_value(int key, char *value1, int *N_value2, double *V_value2)
{

    // Inicializar la colas de mensajes
    mqd_t mq_msg;
    mqd_t mq_res;

    // Definir la cola para la respuesta del servidor
    // Nombre
    char queue_name[MAX_LENGTH];
    sprintf(queue_name, "/Cola-%d", getpid());

    // Atributos
    struct mq_attr attr;
    attr.mq_maxmsg = 1;
    attr.mq_msgsize = sizeof(struct Respuesta);

    // Abrir las colas
    mq_res = mq_open(queue_name, O_CREAT | O_RDONLY, 0700, &attr);
    if (mq_res == -1)
    { // Controlar errores al abrir las colas
        perror("Error al crear la cola de respuesta");
        exit(1);
    }
    mq_msg = mq_open(QUEUE_NAME, O_WRONLY);
    if (mq_msg == -1)
    { // Controlar errores al abrir las colas
        mq_close(mq_res);
        mq_unlink(queue_name);
        perror("Error al abrir la cola del servidor");
        exit(1);
    }

    // Producir dato
    struct Mensaje msg;

    strcpy(msg.op, GET);           // Clave de operación
    msg.key = key;                 // Key
    strcpy(msg.queue, queue_name); // Nombre de la cola de respuesta

    // Enviar mensaje producido
    printf("Enviando mensaje\n");

    if (mq_send(mq_msg, (const char *)&msg, sizeof(msg), 0) < 0)
    {
        perror("Error al mandar el mensaje");
        mq_close(mq_msg);
        mq_close(mq_res);
        mq_unlink(queue_name);
        exit(1);
    }

    // Leer respuesta del servidor
    struct Respuesta res;

    printf("Esperando respuesta del servidor\n");
    if (mq_receive(mq_res, (char *)&res, sizeof(res), 0) == -1)
    {
        perror("Error al recibir el mensaje");
        mq_close(mq_msg);
        mq_close(mq_res);
        mq_unlink(queue_name);
        exit(1);
    }
    printf("Respuesta: %d\n", res.codigo);
    // Devolver los datos recuperados a los punteros del cliente
    if (res.codigo >= 0)
    {
        strcpy(value1, res.cadena);
        *N_value2 = res.N;
        memcpy(V_value2, res.vector, *N_value2 * sizeof(double));
    }

    // Cerrar las colas de mensajes
    mq_close(mq_msg);
    mq_close(mq_res);
    mq_unlink(queue_name);
    return res.codigo;
}

int modify_value(int key, char *value1, int N_value2, double *V_value2)
{

    // Inicializar la colas de mensajes
    mqd_t mq_msg;
    mqd_t mq_res;

    // Definir la cola para la respuesta del servidor
    // Nombre
    char queue_name[MAX_LENGTH];
    sprintf(queue_name, "/Cola-%d", getpid());

    // Atributos
    struct mq_attr attr;
    attr.mq_maxmsg = 1;
    attr.mq_msgsize = sizeof(struct Respuesta);

    // Abrir las colas
    mq_res = mq_open(queue_name, O_CREAT | O_RDONLY, 0700, &attr);
    if (mq_res == -1)
    { // Controlar errores al abrir las colas
        perror("Error al crear la cola de respuesta");
        exit(1);
    }
    mq_msg = mq_open(QUEUE_NAME, O_WRONLY);
    if (mq_msg == -1)
    { // Controlar errores al abrir las colas
        mq_close(mq_res);
        mq_unlink(queue_name);
        perror("Error al abrir la cola del servidor");
        exit(1);
    }

    // Producir dato
    struct Mensaje msg;

    strcpy(msg.op, MODIFY);                                  // Clave de operación
    msg.key = key;                                           // Key
    strcpy(msg.cadena, value1);                              // Cadena
    msg.N = N_value2;                                        // N
    memcpy(msg.vector, V_value2, N_value2 * sizeof(double)); // Vector de doubles
    strcpy(msg.queue, queue_name);                           // Nombre de la cola de respuesta

    // Enviar mensaje producido
    printf("Enviando mensaje\n");

    if (mq_send(mq_msg, (const char *)&msg, sizeof(msg), 0) < 0)
    {
        perror("Error al mandar el mensaje");
        mq_close(mq_msg);
        mq_close(mq_res);
        mq_unlink(queue_name);
        exit(1);
    }

    // Leer respuesta del servidor
    struct Respuesta res;

    printf("Esperando respuesta del servidor\n");
    if (mq_receive(mq_res, (char *)&res, sizeof(res), 0) == -1)
    {
        perror("Error al recibir el mensaje");
        mq_close(mq_msg);
        mq_close(mq_res);
        mq_unlink(queue_name);
        exit(1);
    }
    printf("Respuesta: %d\n", res.codigo);

    // Cerrar las colas de mensajes
    mq_close(mq_msg);
    mq_close(mq_res);
    mq_unlink(queue_name);
    return res.codigo;
}

int delete_key(int key)
{

    // Inicializar la colas de mensajes
    mqd_t mq_msg;
    mqd_t mq_res;

    // Definir la cola para la respuesta del servidor
    // Nombre
    char queue_name[MAX_LENGTH];
    sprintf(queue_name, "/Cola-%d", getpid());

    // Atributos
    struct mq_attr attr;
    attr.mq_maxmsg = 1;
    attr.mq_msgsize = sizeof(struct Respuesta);

    // Abrir las colas
    mq_res = mq_open(queue_name, O_CREAT | O_RDONLY, 0700, &attr);
    if (mq_res == -1)
    { // Controlar errores al abrir las colas
        perror("Error al crear la cola de respuesta");
        exit(1);
    }
    mq_msg = mq_open(QUEUE_NAME, O_WRONLY);
    if (mq_msg == -1)
    { // Controlar errores al abrir las colas
        mq_close(mq_res);
        mq_unlink(queue_name);
        perror("Error al abrir la cola del servidor");
        exit(1);
    }

    // Producir dato
    struct Mensaje msg;

    strcpy(msg.op, DELETE);        // Clave de operación
    msg.key = key;                 // Key
    strcpy(msg.queue, queue_name); // Nombre de la cola de respuesta

    // Enviar mensaje producido
    printf("Enviando mensaje\n");

    if (mq_send(mq_msg, (const char *)&msg, sizeof(msg), 0) < 0)
    {
        perror("Error al mandar el mensaje");
        mq_close(mq_msg);
        mq_close(mq_res);
        mq_unlink(queue_name);
        exit(1);
    }

    // Leer respuesta del servidor
    struct Respuesta res;

    printf("Esperando respuesta del servidor\n");
    if (mq_receive(mq_res, (char *)&res, sizeof(res), 0) == -1)
    {
        perror("Error al recibir el mensaje");
        mq_close(mq_msg);
        mq_close(mq_res);
        mq_unlink(queue_name);
        exit(1);
    }
    printf("Respuesta: %d\n", res.codigo);

    // Cerrar las colas de mensajes
    mq_close(mq_msg);
    mq_close(mq_res);
    mq_unlink(queue_name);
    return res.codigo;
}

int exist(int key)
{

    // Inicializar la colas de mensajes
    mqd_t mq_msg;
    mqd_t mq_res;

    // Definir la cola para la respuesta del servidor
    // Nombre
    char queue_name[MAX_LENGTH];
    sprintf(queue_name, "/Cola-%d", getpid());

    // Atributos
    struct mq_attr attr;
    attr.mq_maxmsg = 1;
    attr.mq_msgsize = sizeof(struct Respuesta);

    // Abrir las colas
    mq_res = mq_open(queue_name, O_CREAT | O_RDONLY, 0700, &attr);
    if (mq_res == -1)
    { // Controlar errores al abrir las colas
        perror("Error al crear la cola de respuesta");
        exit(1);
    }
    mq_msg = mq_open(QUEUE_NAME, O_WRONLY);
    if (mq_msg == -1)
    { // Controlar errores al abrir las colas
        mq_close(mq_res);
        mq_unlink(queue_name);
        perror("Error al abrir la cola del servidor");
        exit(1);
    }

    // Producir dato
    struct Mensaje msg;

    strcpy(msg.op, EXIST);         // Clave de operación
    msg.key = key;                 // Key
    strcpy(msg.queue, queue_name); // Nombre de la cola de respuesta

    // Enviar mensaje producido
    printf("Enviando mensaje\n");

    if (mq_send(mq_msg, (const char *)&msg, sizeof(msg), 0) < 0)
    {
        perror("Error al mandar el mensaje");
        mq_close(mq_msg);
        mq_close(mq_res);
        mq_unlink(queue_name);
        exit(1);
    }

    // Leer respuesta del servidor
    struct Respuesta res;

    printf("Esperando respuesta del servidor\n");
    if (mq_receive(mq_res, (char *)&res, sizeof(res), 0) == -1)
    {
        perror("Error al recibir el mensaje");
        mq_close(mq_msg);
        mq_close(mq_res);
        mq_unlink(queue_name);
        exit(1);
    }
    printf("Respuesta: %d\n", res.codigo);

    // Cerrar las colas de mensajes
    mq_close(mq_msg);
    mq_close(mq_res);
    mq_unlink(queue_name);
    return res.codigo;
}
