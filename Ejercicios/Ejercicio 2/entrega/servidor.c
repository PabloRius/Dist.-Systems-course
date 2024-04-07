#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>
#include <stdbool.h>

#include "ABB.h"
#include "utils.h"
#include "comm.h"

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

void init(void *arg)
{
    printf("Servicio init\n");

    // Inicializamos la respuesta
    int ret;
    int32_t res = 0;

    // Inicializamos el socket local para el hilo
    int s_local;

    // Bloqueamos el lock para guardar el mensaje
    pthread_mutex_lock(&mutex_mensaje);

    s_local = (*(int *)arg);

    mensaje_no_copiado = false;
    // Despertamos al hilo principal
    pthread_cond_signal(&cond_mensaje);
    // Desbloqueamos el lock
    pthread_mutex_unlock(&mutex_mensaje);

    // Comprobar que la estructura de datos no está poblada / reiniciarla
    if (tree != NULL)
    {
        pthread_mutex_lock(&mutex_abb);
        free(tree);
        pthread_mutex_unlock(&mutex_abb);
    }

    // Iniciar la estructura de datos
    pthread_mutex_lock(&mutex_abb);
    if (init_tree(&tree) == -1)
    {
        printf("Error creando inicializando el árbol\n");
        res = -1;
    }
    else
    {
        initialized = 1;
        printf("Árbol inicializado\n");
    }
    pthread_mutex_unlock(&mutex_abb);

    res = htonl(res);

    ret = sendMessage(s_local, (char *)&res, sizeof(int32_t));
    if (ret == -1)
    {
        printf("Error en el envío\n");

        close(s_local);
        pthread_exit(NULL);
        exit(-1);
    }
    close(s_local);
    pthread_exit(NULL);
    exit(-1);
}

void get_tuple(void *arg)
{
    printf("Servicio get\n");

    // Inicializamos la respuesta
    int ret;
    int32_t res = 0;

    // Inicializamos el socket local para el hilo
    int s_local;

    // Bloqueamos el lock para guardar el mensaje
    pthread_mutex_lock(&mutex_mensaje);

    s_local = (*(int *)arg);

    mensaje_no_copiado = false;
    // Despertamos al hilo principal
    pthread_cond_signal(&cond_mensaje);
    // Desbloqueamos el lock
    pthread_mutex_unlock(&mutex_mensaje);

    // Comprobar que la estructura de datos está poblada
    if (initialized == 0)
    {
        close(s_local);
        pthread_exit(NULL);
        exit(-1);
    }

    // Recibimos la key en el siguiente mensaje por el socket
    int32_t key;
    char new_cadena[255];
    int32_t new_N;
    double new_vector[32];

    ret = recvMessage(s_local, (char *)&key, sizeof(int32_t));
    if (ret < 0)
    {
        printf("Error en la recepción de la clave\n");

        close(s_local);
        pthread_exit(NULL);
        exit(-1);
    }
    key = ntohl(key);

    // Buscamos el nodo con la clave recibida
    pthread_mutex_lock(&mutex_abb);
    if (get_node(tree, key, new_cadena, &new_N, new_vector) == -1)
    {
        printf("Error buscando el nodo: %d\n", key);
        res = -1;
    }
    else
    {
        printf("Nodo encontrado\n");
    }
    pthread_mutex_unlock(&mutex_abb);
    if (res == -1)
    {
        res = htonl(res);
        ret = sendMessage(s_local, (char *)&res, sizeof(int32_t));
        if (ret == -1)
        {
            printf("Error en el envío\n");

            close(s_local);
            pthread_exit(NULL);
            exit(-1);
        }
    }
    else
    {
        res = htonl(res);
        ret = sendMessage(s_local, (char *)&res, sizeof(int32_t));
        if (ret == -1)
        {
            printf("Error en el envío\n");

            close(s_local);
            pthread_exit(NULL);
            exit(-1);
        }
        ret = sendMessage(s_local, (char *)new_cadena, 255 * sizeof(char));
        if (ret == -1)
        {
            printf("Error en el envío\n");

            close(s_local);
            pthread_exit(NULL);
            exit(-1);
        }
        int32_t N = htonl(new_N);
        ret = sendMessage(s_local, (char *)&N, sizeof(int32_t));
        if (ret == -1)
        {
            printf("Error en el envío\n");

            close(s_local);
            pthread_exit(NULL);
            exit(-1);
        }
        char vector_to_array[102 * new_N];
        datoc(new_vector, new_N, vector_to_array);
        ret = sendMessage(s_local, (char *)vector_to_array, 102 * new_N * sizeof(char));
        if (ret == -1)
        {
            printf("Error en el envío\n");

            close(s_local);
            pthread_exit(NULL);
            exit(-1);
        }
    }
    close(s_local);
    pthread_exit(NULL);
    exit(-1);
}

void set_tuple(void *arg)
{
    printf("Servicio set\n");

    // Inicializamos la respuesta
    int ret;
    int32_t res = 0;

    // Inicializamos el socket local para el hilo
    int s_local;

    // Bloqueamos el lock para guardar el mensaje
    pthread_mutex_lock(&mutex_mensaje);

    s_local = (*(int *)arg);

    mensaje_no_copiado = false;
    // Despertamos al hilo principal
    pthread_cond_signal(&cond_mensaje);
    // Desbloqueamos el lock
    pthread_mutex_unlock(&mutex_mensaje);

    // Comprobar que la estructura de datos está poblada
    if (initialized == 0)
    {
        close(s_local);
        pthread_exit(NULL);
        exit(-1);
    }

    // Recibimos la key en el siguiente mensaje por el socket
    int32_t key;
    char cadena[255];
    int32_t N;

    ret = recvMessage(s_local, (char *)&key, sizeof(int32_t));
    if (ret < 0)
    {
        printf("Error en la recepción de la clave\n");

        close(s_local);
        pthread_exit(NULL);
        exit(-1);
    }
    key = ntohl(key);
    ret = recvMessage(s_local, (char *)cadena, 255 * sizeof(char));
    if (ret < 0)
    {
        printf("Error en la recepción de la cadena\n");

        close(s_local);
        pthread_exit(NULL);
        exit(-1);
    }
    ret = recvMessage(s_local, (char *)&N, sizeof(int32_t));
    if (ret < 0)
    {
        printf("Error en la recepción de N\n");

        close(s_local);
        pthread_exit(NULL);
        exit(-1);
    }
    N = ntohl(N);
    char char_double_array[102 * N];
    ret = recvMessage(s_local, (char *)char_double_array, 102 * N * sizeof(char));
    if (ret < 0)
    {
        printf("Error en la recepción de la cadena\n");

        close(s_local);
        pthread_exit(NULL);
        exit(-1);
    }
    double vector[N];
    ctoda(vector, N, char_double_array);

    // Introducimos el nodo con la clave recibida
    pthread_mutex_lock(&mutex_abb);
    if (post_node(tree, key, cadena, N, vector) == -1)
    {
        printf("Error introduciendo el nodo: %d\n", key);
        res = -1;
    }
    else
    {
        printf("Nodo introducido\n");
        print_tree(tree, 1);
    }
    pthread_mutex_unlock(&mutex_abb);

    res = htonl(res);
    ret = sendMessage(s_local, (char *)&res, sizeof(int32_t));
    if (ret == -1)
    {
        printf("Error en el envío\n");

        close(s_local);
        pthread_exit(NULL);
        exit(-1);
    }

    close(s_local);
    pthread_exit(NULL);
    exit(-1);
}

void modify_tuple(void *arg)
{
    printf("Servicio modify\n");

    // Inicializamos la respuesta
    int ret;
    int32_t res = 0;

    // Inicializamos el socket local para el hilo
    int s_local;

    // Bloqueamos el lock para guardar el mensaje
    pthread_mutex_lock(&mutex_mensaje);

    s_local = (*(int *)arg);

    mensaje_no_copiado = false;
    // Despertamos al hilo principal
    pthread_cond_signal(&cond_mensaje);
    // Desbloqueamos el lock
    pthread_mutex_unlock(&mutex_mensaje);

    // Comprobar que la estructura de datos está poblada
    if (initialized == 0)
    {
        close(s_local);
        pthread_exit(NULL);
        exit(-1);
    }

    // Recibimos la key en el siguiente mensaje por el socket
    int32_t key;
    char cadena[255];
    int32_t N;

    ret = recvMessage(s_local, (char *)&key, sizeof(int32_t));
    if (ret < 0)
    {
        printf("Error en la recepción de la clave\n");

        close(s_local);
        pthread_exit(NULL);
        exit(-1);
    }
    key = ntohl(key);
    ret = recvMessage(s_local, (char *)cadena, 255 * sizeof(char));
    if (ret < 0)
    {
        printf("Error en la recepción de la cadena\n");

        close(s_local);
        pthread_exit(NULL);
        exit(-1);
    }
    ret = recvMessage(s_local, (char *)&N, sizeof(int32_t));
    if (ret < 0)
    {
        printf("Error en la recepción de N\n");

        close(s_local);
        pthread_exit(NULL);
        exit(-1);
    }
    N = ntohl(N);
    char char_double_array[102 * N];
    ret = recvMessage(s_local, (char *)char_double_array, 102 * N * sizeof(char));
    if (ret < 0)
    {
        printf("Error en la recepción de la cadena\n");

        close(s_local);
        pthread_exit(NULL);
        exit(-1);
    }
    double vector[N];
    ctoda(vector, N, char_double_array);

    // Modifcamos el nodo con la clave recibida
    pthread_mutex_lock(&mutex_abb);
    if (patch_node(tree, key, cadena, N, vector) == -1)
    {
        printf("Error modificando el nodo: %d\n", key);
        res = -1;
    }
    else
    {
        printf("Nodo modificado\n");
        print_tree(tree, 1);
    }
    pthread_mutex_unlock(&mutex_abb);

    res = htonl(res);
    ret = sendMessage(s_local, (char *)&res, sizeof(int32_t));
    if (ret == -1)
    {
        printf("Error en el envío\n");

        close(s_local);
        pthread_exit(NULL);
        exit(-1);
    }

    close(s_local);
    pthread_exit(NULL);
    exit(-1);
}

void delete_tuple(void *arg)
{
    printf("Servicio delete\n");

    // Inicializamos la respuesta
    int ret;
    int32_t res = 0;

    // Inicializamos el socket local para el hilo
    int s_local;

    // Bloqueamos el lock para guardar el mensaje
    pthread_mutex_lock(&mutex_mensaje);

    s_local = (*(int *)arg);

    mensaje_no_copiado = false;
    // Despertamos al hilo principal
    pthread_cond_signal(&cond_mensaje);
    // Desbloqueamos el lock
    pthread_mutex_unlock(&mutex_mensaje);

    // Comprobar que la estructura de datos está poblada
    if (initialized == 0)
    {
        close(s_local);
        pthread_exit(NULL);
        exit(-1);
    }

    // Recibimos la key en el siguiente mensaje por el socket
    int32_t key;

    ret = recvMessage(s_local, (char *)&key, sizeof(int32_t));
    if (ret < 0)
    {
        printf("Error en la recepción de la clave\n");

        close(s_local);
        pthread_exit(NULL);
        exit(-1);
    }
    key = ntohl(key);

    // Eliminamos el nodo con la clave recibida
    pthread_mutex_lock(&mutex_abb);
    if (delete_node(tree, key) == -1)
    {
        printf("Error eliminando el nodo: %d\n", key);
        res = -1;
    }
    else
    {
        printf("Nodo eliminado\n");
        print_tree(tree, 1);
    }
    pthread_mutex_unlock(&mutex_abb);

    res = htonl(res);
    ret = sendMessage(s_local, (char *)&res, sizeof(int32_t));
    if (ret == -1)
    {
        printf("Error en el envío\n");

        close(s_local);
        pthread_exit(NULL);
        exit(-1);
    }

    close(s_local);
    pthread_exit(NULL);
    exit(-1);
}

void exist_tuple(void *arg)
{
    printf("Servicio exist\n");

    // Inicializamos la respuesta
    int ret;
    int32_t res = 0;

    // Inicializamos el socket local para el hilo
    int s_local;

    // Bloqueamos el lock para guardar el mensaje
    pthread_mutex_lock(&mutex_mensaje);

    s_local = (*(int *)arg);

    mensaje_no_copiado = false;
    // Despertamos al hilo principal
    pthread_cond_signal(&cond_mensaje);
    // Desbloqueamos el lock
    pthread_mutex_unlock(&mutex_mensaje);

    // Comprobar que la estructura de datos está poblada
    if (initialized == 0)
    {
        close(s_local);
        pthread_exit(NULL);
        exit(-1);
    }

    // Recibimos la key en el siguiente mensaje por el socket
    int32_t key;

    ret = recvMessage(s_local, (char *)&key, sizeof(int32_t));
    if (ret < 0)
    {
        printf("Error en la recepción de la clave\n");

        close(s_local);
        pthread_exit(NULL);
        exit(-1);
    }
    key = ntohl(key);

    // Buscamos el nodo con la clave recibida
    pthread_mutex_lock(&mutex_abb);
    res = head_node(tree, key);
    pthread_mutex_unlock(&mutex_abb);

    res = htonl(res);
    ret = sendMessage(s_local, (char *)&res, sizeof(int32_t));
    if (ret == -1)
    {
        printf("Error en el envío\n");

        close(s_local);
        pthread_exit(NULL);
        exit(-1);
    }

    close(s_local);
    pthread_exit(NULL);
    exit(-1);
}

int get_servicio(int sc)
{
    int ret;
    char op;

    ret = recvMessage(sc, (char *)&op, sizeof(char));
    if (ret < 0)
    {
        printf("Error en la recepción de la operación\n");
        return -1;
    }
    printf("Recibida operación: %d\n", op);

    return op;
}

int the_end = 0;

void sigHandler(int signo)
{
    the_end = 1;
}

int main(int argc, char **argv)
{
    printf("Ha iniciado el servidor\n");
    // Leemos el argumento que necesitamos para el puerto
    if (argc != 2)
    {
        printf("Uso: %s <puerto servidor>\n", argv[0]);
        printf("Ejemplo -> %s localhost 4200\n", argv[0]);
        return -1;
    }
    if (argv[1] == NULL)
    {
        printf("Puerto no definido\n");
        printf("Ejemplo -> %s localhost 4200\n", argv[0]);
        return -1;
    }
    char *endptr;
    long port = strtol(argv[1], &endptr, 10);
    if (*endptr != '\0')
    {
        printf("El valor de Puerto no es un número entero.\n");
        printf("Ejemplo -> %s localhost 4200\n", argv[0]);
        return -1;
    }

    // Definir el socket
    int sd, sc;
    struct sigaction new_action, old_action;

    sd = serverSocket(INADDR_ANY, port, SOCK_STREAM);
    if (sd < 0)
    {
        printf("SERVER: Error al crear el socket\n");
        return 0;
    }

    // si se presiona Ctrl-C el bucle termina
    new_action.sa_handler = sigHandler;
    sigemptyset(&new_action.sa_mask);
    new_action.sa_flags = 0;
    sigaction(SIGINT, NULL, &old_action);
    if (old_action.sa_handler != SIG_IGN)
    {
        sigaction(SIGINT, &new_action, NULL);
    }

    // atributos de los hilos

    pthread_t thid;
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    // Leer las peticiones en un bucle infinito
    while (0 == the_end)
    {
        // Leer una petición
        sc = serverAccept(sd);
        if (sc < 0)
        {
            printf("Error en el accept\n");
            continue;
        }

        int servicio = get_servicio(sc);

        // Hacer un switch según el código de operación

        switch (servicio)
        {
        case INIT:
            if (pthread_create(&thid, &attr, (void *)init, (void *)&sc) == 0)
            {
                pthread_mutex_lock(&mutex_mensaje);
                while (mensaje_no_copiado)
                    pthread_cond_wait(&cond_mensaje, &mutex_mensaje);
                mensaje_no_copiado = true;
                pthread_mutex_unlock(&mutex_mensaje);
            }
            break;

        case GET:
            if (pthread_create(&thid, &attr, (void *)get_tuple, (void *)&sc) == 0)
            {
                pthread_mutex_lock(&mutex_mensaje);
                while (mensaje_no_copiado)
                    pthread_cond_wait(&cond_mensaje, &mutex_mensaje);
                mensaje_no_copiado = true;
                pthread_mutex_unlock(&mutex_mensaje);
            }
            break;

        case SET:
            if (pthread_create(&thid, &attr, (void *)set_tuple, (void *)&sc) == 0)
            {
                pthread_mutex_lock(&mutex_mensaje);
                while (mensaje_no_copiado)
                    pthread_cond_wait(&cond_mensaje, &mutex_mensaje);
                mensaje_no_copiado = true;
                pthread_mutex_unlock(&mutex_mensaje);
            }
            break;

        case MODIFY:
            if (pthread_create(&thid, &attr, (void *)modify_tuple, (void *)&sc) == 0)
            {
                pthread_mutex_lock(&mutex_mensaje);
                while (mensaje_no_copiado)
                    pthread_cond_wait(&cond_mensaje, &mutex_mensaje);
                mensaje_no_copiado = true;
                pthread_mutex_unlock(&mutex_mensaje);
            }
            break;

        case DELETE:
            if (pthread_create(&thid, &attr, (void *)delete_tuple, (void *)&sc) == 0)
            {
                pthread_mutex_lock(&mutex_mensaje);
                while (mensaje_no_copiado)
                    pthread_cond_wait(&cond_mensaje, &mutex_mensaje);
                mensaje_no_copiado = true;
                pthread_mutex_unlock(&mutex_mensaje);
            }
            break;

        case EXIST:
            if (pthread_create(&thid, &attr, (void *)exist_tuple, (void *)&sc) == 0)
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
            printf("Error al registrar el número de operación: %d\n", servicio);
            closeSocket(sc);

            break;
        }
    }

    // Cerrar el socket al finalizar el programa
    closeSocket(sd);

    return 0;
}