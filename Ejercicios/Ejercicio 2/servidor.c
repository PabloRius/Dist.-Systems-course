#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>
#include <stdbool.h>

#include "ABB.h"
#include "utils.h"
#include "mensaje.h"

#define INIT 0
#define GET 1
#define SET 2
#define MODIFY 3
#define DELETE 4
#define EXIST 5

#define MAX_BUFFER 10           /* tamaño del buffer */
#define DATOS_A_PRODUCIR 100000 /* datos a producir */

// Control de acceso a la variable mensaje
pthread_mutex_t mutex_mensaje;
int mensaje_no_copiado = true;
pthread_cond_t cond_mensaje;

// Control de acceso a la estructura de datos
pthread_mutex_t mutex_abb;

struct Tree *tree;
int initialized = 0;

void init(void *msg)
{
    printf("Serivicio init\n");

    // Definir el mensaje de respuesta
    struct Respuesta res;
    res.codigo = 0;

    // Bloqueamos el lock para guardar el mensaje
    pthread_mutex_lock(&mutex_mensaje);
    // Copiamos el mensaje
    struct Mensaje mensaje;
    mensaje = (*(struct Mensaje *)msg);

    mensaje_no_copiado = false;
    // Despertamos al hilo principal
    pthread_cond_signal(&cond_mensaje);
    // Desbloqueamos el lock
    pthread_mutex_unlock(&mutex_mensaje);

    // Comprobar que la estructura de datos no está poblada / reiniciarla
    if (tree != NULL)
    {
        // TODO eliminar el arbol y todos sus nodos correctamente
        pthread_mutex_lock(&mutex_abb);
        free(tree);
        pthread_mutex_unlock(&mutex_abb);
    }

    // Iniciar la estructura de datos
    pthread_mutex_lock(&mutex_abb);
    if (init_tree(&tree) == -1)
    {
        printf("Error creando inicializando el árbol\n");
        res.codigo = -1;
    }
    else
    {
        initialized = 1;
    }
    pthread_mutex_unlock(&mutex_abb);

    // Abrir la cola del cliente para mandar la respuesta
    mqd_t mq;
    mq = mq_open(mensaje.queue, O_WRONLY);
    if (mq == -1)
    {
        perror("Error al abrir la cola del cliente");
        exit(-1);
    }
    else
    {
        // Mandar la respuesta por la cola
        if (mq_send(mq, (const char *)&res, sizeof(res), 0) < 0)
        {
            perror("Error al mandar el mensaje de respuesta");
            mq_close(mq);
            exit(1);
        }

        // Cerrar la conexion con la cola
        mq_close(mq);
    }
}

void get_tuple(void *msg)
{
    printf("Servicio get\n");

    // Definir el mensaje de respuesta
    struct Respuesta res;
    res.codigo = 0;

    // Bloqueamos el lock para guardar el mensaje
    pthread_mutex_lock(&mutex_mensaje);
    // Copiamos el mensaje
    struct Mensaje mensaje;
    mensaje = (*(struct Mensaje *)msg);

    mensaje_no_copiado = false;
    // Despertamos al hilo principal
    pthread_cond_signal(&cond_mensaje);
    // Desbloqueamos el lock
    pthread_mutex_unlock(&mutex_mensaje);

    // Abrir la cola del cliente para mandar la respuesta
    mqd_t mq;
    mq = mq_open(mensaje.queue, O_WRONLY);
    if (mq == -1)
    {
        perror("Error al abrir la cola del cliente");
        exit(-1);
    }
    else
    {
        // Comprobar que la estructura de datos está inicializada
        pthread_mutex_lock(&mutex_abb);
        if (initialized == 1)
        {
            // Realizar la operación
            get_node(tree, mensaje.key, res.cadena, &res.N, res.vector);
        }
        else
        {
            res.codigo = -1;
        }
        pthread_mutex_unlock(&mutex_abb);

        // Mandar la respuesta por la cola
        if (mq_send(mq, (const char *)&res, sizeof(res), 0) < 0)
        {
            perror("Error al mandar el mensaje de respuesta");
            mq_close(mq);
            exit(1);
        }

        // Cerrar la conexion con la cola
        mq_close(mq);
    }
}

void set_tuple(void *msg)
{
    printf("Servicio set\n");

    // Definir el mensaje de respuesta
    struct Respuesta res;
    res.codigo = 0;

    // Bloqueamos el lock para guardar el mensaje
    pthread_mutex_lock(&mutex_mensaje);
    // Copiamos el mensaje
    struct Mensaje mensaje;
    mensaje = (*(struct Mensaje *)msg);

    mensaje_no_copiado = false;
    // Despertamos al hilo principal
    pthread_cond_signal(&cond_mensaje);
    // Desbloqueamos el lock
    pthread_mutex_unlock(&mutex_mensaje);

    // Abrir la cola del cliente para mandar la respuesta
    mqd_t mq;
    mq = mq_open(mensaje.queue, O_WRONLY);
    if (mq == -1)
    {
        perror("Error al abrir la cola del cliente");
        exit(-1);
    }
    else
    {
        // Comprobar que la estructura de datos está inicializada
        pthread_mutex_lock(&mutex_abb);
        if (initialized == 1)
        {
            // Realizar la operación
            if (post_node(tree, mensaje.key, mensaje.cadena, mensaje.N, mensaje.vector) == -1)
            {
                res.codigo = -1;
            }
            print_tree(tree, 1);
        }
        else
        {
            res.codigo = -1;
        }
        pthread_mutex_unlock(&mutex_abb);

        // Mandar la respuesta por la cola
        if (mq_send(mq, (const char *)&res, sizeof(res), 0) < 0)
        {
            perror("Error al mandar el mensaje de respuesta");
            mq_close(mq);
            exit(1);
        }

        // Cerrar la conexion con la cola
        mq_close(mq);
    }
}

void modify_tuple(void *msg)
{
    printf("Servicio modify\n");

    // Definir el mensaje de respuesta
    struct Respuesta res;
    res.codigo = 0;

    // Bloqueamos el lock para guardar el mensaje
    pthread_mutex_lock(&mutex_mensaje);
    // Copiamos el mensaje
    struct Mensaje mensaje;
    mensaje = (*(struct Mensaje *)msg);

    mensaje_no_copiado = false;
    // Despertamos al hilo principal
    pthread_cond_signal(&cond_mensaje);
    // Desbloqueamos el lock
    pthread_mutex_unlock(&mutex_mensaje);

    // Abrir la cola del cliente para mandar la respuesta
    mqd_t mq;
    mq = mq_open(mensaje.queue, O_WRONLY);
    if (mq == -1)
    {
        perror("Error al abrir la cola del cliente");
        exit(-1);
    }
    else
    {
        // Comprobar que la estructura de datos está inicializada
        pthread_mutex_lock(&mutex_abb);
        if (initialized == 1)
        {
            // Realizar la operación
            if (patch_node(tree, mensaje.key, mensaje.cadena, mensaje.N, mensaje.vector) == -1)
            {
                res.codigo = -1;
            }
            print_tree(tree, 1);
        }
        else
        {
            res.codigo = -1;
        }
        pthread_mutex_unlock(&mutex_abb);

        // Mandar la respuesta por la cola
        if (mq_send(mq, (const char *)&res, sizeof(res), 0) < 0)
        {
            perror("Error al mandar el mensaje de respuesta");
            mq_close(mq);
            exit(1);
        }

        // Cerrar la conexion con la cola
        mq_close(mq);
    }
}

void delete_tuple(void *msg)
{
    printf("Servicio delete\n");

    // Definir el mensaje de respuesta
    struct Respuesta res;
    res.codigo = 0;

    // Bloqueamos el lock para guardar el mensaje
    pthread_mutex_lock(&mutex_mensaje);
    // Copiamos el mensaje
    struct Mensaje mensaje;
    mensaje = (*(struct Mensaje *)msg);

    mensaje_no_copiado = false;
    // Despertamos al hilo principal
    pthread_cond_signal(&cond_mensaje);
    // Desbloqueamos el lock
    pthread_mutex_unlock(&mutex_mensaje);

    // Abrir la cola del cliente para mandar la respuesta
    mqd_t mq;
    mq = mq_open(mensaje.queue, O_WRONLY);
    if (mq == -1)
    {
        perror("Error al abrir la cola del cliente");
        exit(-1);
    }
    else
    {
        // Comprobar que la estructura de datos está inicializada
        pthread_mutex_lock(&mutex_abb);
        if (initialized == 1)
        {
            // Realizar la operación
            if (delete_node(tree, mensaje.key) == -1)
            {
                res.codigo = -1;
            }
            print_tree(tree, 1);
        }
        else
        {
            res.codigo = -1;
        }
        pthread_mutex_unlock(&mutex_abb);

        // Mandar la respuesta por la cola
        if (mq_send(mq, (const char *)&res, sizeof(res), 0) < 0)
        {
            perror("Error al mandar el mensaje de respuesta");
            mq_close(mq);
            exit(1);
        }

        // Cerrar la conexion con la cola
        mq_close(mq);
    }
}

void exist_tuple(void *msg)
{
    printf("Servicio exist\n");

    // Definir el mensaje de respuesta
    struct Respuesta res;
    res.codigo = 0;

    // Bloqueamos el lock para guardar el mensaje
    pthread_mutex_lock(&mutex_mensaje);
    // Copiamos el mensaje
    struct Mensaje mensaje;
    mensaje = (*(struct Mensaje *)msg);

    mensaje_no_copiado = false;
    // Despertamos al hilo principal
    pthread_cond_signal(&cond_mensaje);
    // Desbloqueamos el lock
    pthread_mutex_unlock(&mutex_mensaje);

    // Abrir la cola del cliente para mandar la respuesta
    mqd_t mq;
    mq = mq_open(mensaje.queue, O_WRONLY);
    if (mq == -1)
    {
        perror("Error al abrir la cola del cliente");
        exit(-1);
    }
    else
    {
        // Comprobar que la estructura de datos está inicializada
        pthread_mutex_lock(&mutex_abb);
        if (initialized == 1)
        {
            // Realizar la operación
            res.codigo = head_node(tree, mensaje.key);
        }
        else
        {
            res.codigo = -1;
        }
        pthread_mutex_unlock(&mutex_abb);

        // Mandar la respuesta por la cola
        if (mq_send(mq, (const char *)&res, sizeof(res), 0) < 0)
        {
            perror("Error al mandar el mensaje de respuesta");
            mq_close(mq);
            exit(1);
        }

        // Cerrar la conexion con la cola
        mq_close(mq);
    }
}

int main(int argc, char **argv)
{
    printf("Ha iniciado el servidor\n");

    // Definir la cola de mensajes para las peticiones
    mqd_t mq;
    // Atributos
    struct mq_attr attr;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(struct Mensaje);

    // Abrir la cola de mensajes
    mq = mq_open("/tuple_sv_queue", O_CREAT | O_RDONLY, 0700, &attr);
    if (mq == -1)
    {
        perror("mq_open");
        exit(-1);
    }
    pthread_t thid;
    pthread_attr_t t_attr;

    pthread_mutex_init(&mutex_mensaje, NULL);
    pthread_cond_init(&cond_mensaje, NULL);
    pthread_attr_init(&t_attr);
    pthread_attr_setdetachstate(&t_attr, PTHREAD_CREATE_DETACHED);

    // Leer el las peticiones en un bucle infinito
    struct Mensaje msg;
    for (;;)
    {
        // Leer una petición
        if (mq_receive(mq, (char *)&msg, sizeof(msg), 0) == -1)
        {
            perror("mq_receive");
            mq_close(mq);
            exit(1);
        }
        // Hacer un switch según el código de operación
        char *end;

        switch (strtol(msg.op, &end, 10))
        {
        case INIT:
            if (pthread_create(&thid, &t_attr, (void *)init, (void *)&msg) == 0)
            {
                pthread_mutex_lock(&mutex_mensaje);
                while (mensaje_no_copiado)
                    pthread_cond_wait(&cond_mensaje, &mutex_mensaje);
                mensaje_no_copiado = true;
                pthread_mutex_unlock(&mutex_mensaje);
            }
            break;

        case GET:
            if (pthread_create(&thid, &t_attr, (void *)get_tuple, (void *)&msg) == 0)
            {
                pthread_mutex_lock(&mutex_mensaje);
                while (mensaje_no_copiado)
                    pthread_cond_wait(&cond_mensaje, &mutex_mensaje);
                mensaje_no_copiado = true;
                pthread_mutex_unlock(&mutex_mensaje);
            }
            break;

        case SET:
            if (pthread_create(&thid, &t_attr, (void *)set_tuple, (void *)&msg) == 0)
            {
                pthread_mutex_lock(&mutex_mensaje);
                while (mensaje_no_copiado)
                    pthread_cond_wait(&cond_mensaje, &mutex_mensaje);
                mensaje_no_copiado = true;
                pthread_mutex_unlock(&mutex_mensaje);
            }
            break;

        case MODIFY:
            if (pthread_create(&thid, &t_attr, (void *)modify_tuple, (void *)&msg) == 0)
            {
                pthread_mutex_lock(&mutex_mensaje);
                while (mensaje_no_copiado)
                    pthread_cond_wait(&cond_mensaje, &mutex_mensaje);
                mensaje_no_copiado = true;
                pthread_mutex_unlock(&mutex_mensaje);
            }
            break;

        case DELETE:
            if (pthread_create(&thid, &t_attr, (void *)delete_tuple, (void *)&msg) == 0)
            {
                pthread_mutex_lock(&mutex_mensaje);
                while (mensaje_no_copiado)
                    pthread_cond_wait(&cond_mensaje, &mutex_mensaje);
                mensaje_no_copiado = true;
                pthread_mutex_unlock(&mutex_mensaje);
            }
            break;

        case EXIST:
            if (pthread_create(&thid, &t_attr, (void *)exist_tuple, (void *)&msg) == 0)
            {
                pthread_mutex_lock(&mutex_mensaje);
                while (mensaje_no_copiado)
                    pthread_cond_wait(&cond_mensaje, &mutex_mensaje);
                mensaje_no_copiado = true;
                pthread_mutex_unlock(&mutex_mensaje);
            }
            break;

        // Definir código de operación incorrecto
        default:
            printf("El dato consumido es: %s\n", msg.op);

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
    }

    // Cerrar la cola de mensajes para peticiones
    mq_close(mq);
    mq_unlink("/tuple_sv_queue");

    return (0);
}