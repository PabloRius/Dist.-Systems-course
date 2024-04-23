#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <stdbool.h>
#include <arpa/inet.h>

#include "utils.h"
#include "ABB.h"
#include "comm.h"

#define MAX_OP_SIZE 13              // La cadena de caracteres que representa una operación
                                    // es LIST_CONTENT con longitud 12 + 1 por el '\0' final

struct Tree *tree;

// Definimos los mutex y variables condicionales para la copia de los argumentos pasados a los hilos
pthread_mutex_t m_msg;
int mensaje_no_copiado = true;
pthread_cond_t c_msg;
// Definimos el mutex que controla el acceso a la estructura de datos
pthread_mutex_t m_abb;

int the_end = 0;

void register_svc(void *arg)
{
    /**
     * @brief Esta llamada permite registrar a un usuario nuevo en el sistema del servidor.
     *
     * @return int La función devuelve 0 en caso de éxito, 1 si el usuario ya está registrado, 2 en cualquier otro caso.
     * @retval 0 en caso de exito.
     * @retval 1 si el usuario ya está registrado.
     * @retval 2 en caso de otro error.
     */

    // Variable local en la que se copiará el descriptor del socket recibido
    int s_local;

    // El hilo adquiere el mutex para copiar el mensaje mientras el hilo principal espera a recibir una señal
    pthread_mutex_lock(&m_msg);

    s_local = (*(int *)arg);

    // Avisamos al hilo principal de que ya se ha copiado lo que necesitábamos y puede continuar
    mensaje_no_copiado = false;
    pthread_cond_signal(&c_msg);
    pthread_mutex_unlock(&m_msg);

    // Variable que contiene el valor que se devolverá al cliente como resultado de la operación
    char res = '0';

    // Esperamos a que el cliente mande el resto de argumentos por la conexion que abrió y el hilo ha copiado localmente
    char username[MAX_LENGTH];
    int ret = readLine(s_local, username, MAX_LENGTH);
    if (ret < 0)
    {
        // Error al recibir el mensaje del cliente con el username, le enviamos el código de error 2 y cerramos la conexión y el hilo
        res = '2';
        sendMessage(s_local, &res, sizeof(char));
        close(s_local);
        pthread_exit(NULL);
        exit(1);
    }
    printf("s> OPERATION FROM %s\n", username);
    // Adquirimos el lock para acceder a la estructura de datos e introducir el nuevo usuario
    pthread_mutex_lock(&m_abb);
    int op_res = register_user(tree, username);
    print_tree(tree, 1),
    pthread_mutex_unlock(&m_abb);
    switch (op_res)
    {
    case 0:
        res = '0';
        break;
    
    case 1:
        res = '1';
        break;
    
    case 2:
        res = '2';
        break;
    
    default:
        res = '2';
        break;
    }
    ret = sendMessage(s_local, &res, sizeof(char));
    if (ret < 0)
    {
        res = '2';
        close(s_local);
        pthread_exit(NULL);
        exit(1);
    }

    close(s_local);
    pthread_exit(NULL);
    exit(1);
}

void unregister_svc(void *arg)
{
    /**
     * @brief Esta llamada permite des-registrar a un usuario del sistema.
     *
     * @return int La función devuelve 0 en caso de éxito, 1 si el usuario no existe, 2 en cualquier otro caso.
     * @retval 0 en caso de exito.
     * @retval 1 si el usuario no existe.
     * @retval 2 en caso de otro error.
     */

    // Variable local en la que se copiará el descriptor del socket recibido
    int s_local;

    // El hilo adquiere el mutex para copiar el mensaje mientras el hilo principal espera a recibir una señal
    pthread_mutex_lock(&m_msg);

    s_local = (*(int *)arg);

    // Avisamos al hilo principal de que ya se ha copiado lo que necesitábamos y puede continuar
    mensaje_no_copiado = false;
    pthread_cond_signal(&c_msg);
    pthread_mutex_unlock(&m_msg);

    // Variable que contiene el valor que se devolverá al cliente como resultado de la operación
    char res = '0';

    // Esperamos a que el cliente mande el resto de argumentos por la conexion que abrió y el hilo ha copiado localmente
    char username[MAX_LENGTH];
    int ret = readLine(s_local, username, MAX_LENGTH);
    if (ret < 0)
    {
        // Error al recibir el mensaje del cliente con el username, le enviamos el código de error 2 y cerramos la conexión y el hilo
        res = '2';
        sendMessage(s_local, &res, sizeof(char));
        close(s_local);
        pthread_exit(NULL);
        exit(1);
    }
    printf("s> OPERATION FROM %s\n", username);
    // Adquirimos el lock para acceder a la estructura de datos e introducir el nuevo usuario
    pthread_mutex_lock(&m_abb);
    int op_res = unregister_user(tree, username);
    print_tree(tree, 1);
    pthread_mutex_unlock(&m_abb);
    switch (op_res)
    {
    case 0:
        res = '0';
        break;
    
    case 1:
        res = '1';
        break;
    
    case 2:
        res = '2';
        break;
    
    default:
        res = '2';
        break;
    }
    ret = sendMessage(s_local, &res, sizeof(char));
    if (ret < 0)
    {
        res = '2';
        close(s_local);
        pthread_exit(NULL);
        exit(1);
    }

    close(s_local);
    pthread_exit(NULL);
    exit(1);
}

void connect_svc(void *arg)
{
    /**
     * @brief Esta llamada permite a un usuario establecer su estado como conectado en el sistema de intercambio de ficheros.
     *
     * @return int La función devuelve 0 en caso de éxito, 1 si el usuario no existe, 2 si el usuario ya estaba conectado, 3 en cualquier otro caso.
     * @retval 0 en caso de exito.
     * @retval 1 si el usuario no existe.
     * @retval 2 si el usuario ya está conectado.
     * @retval 3 en caso de cualquier otro error.
     */

    // Variable local en la que se copiará el descriptor del socket recibido
    int s_local;

    // El hilo adquiere el mutex para copiar el mensaje mientras el hilo principal espera a recibir una señal
    pthread_mutex_lock(&m_msg);

    s_local = (*(int *)arg);

    // Avisamos al hilo principal de que ya se ha copiado lo que necesitábamos y puede continuar
    mensaje_no_copiado = false;
    pthread_cond_signal(&c_msg);
    pthread_mutex_unlock(&m_msg);

    // Variable que contiene el valor que se devolverá al cliente como resultado de la operación
    char res = '0';

    // Esperamos a que el cliente mande el resto de argumentos por la conexion que abrió y el hilo ha copiado localmente
    char username[MAX_LENGTH];
    char hostname[MAX_LENGTH];
    char port[MAX_LENGTH];
    int ret = readLine(s_local, username, MAX_LENGTH);
    if (ret < 0)
    {
        // Error al recibir el mensaje del cliente con el username, le enviamos el código de error 3 y cerramos la conexión y el hilo
        res = '3';
        sendMessage(s_local, &res, sizeof(char));
        close(s_local);
        pthread_exit(NULL);
        exit(1);
    }
    ret = readLine(s_local, port, MAX_LENGTH);
    if (ret < 0)
    {
        // Error al recibir el mensaje del cliente con el puerto, le enviamos el código de error 3 y cerramos la conexión y el hilo
        res = '3';
        sendMessage(s_local, &res, sizeof(char));
        close(s_local);
        pthread_exit(NULL);
        exit(1);
    }
    // Convertimos el puerto recibido a un int
    char *endptr;
    long num = strtol(port, &endptr, 10);
    if (*endptr != '\0')
    {
        printf("El valor del puerto recibido no es un número entero.\n");
        res = '3';
        sendMessage(s_local, &res, sizeof(char));
        close(s_local);
        pthread_exit(NULL);
        exit(1);
    }
    // Extraemos la dirección ip del cliente desde el propio socket
    struct sockaddr_in clt_addr;
    socklen_t clt_addrlen = sizeof(clt_addr);
    int retval = getpeername(s_local, (struct sockaddr *)&clt_addr, &clt_addrlen);
    if (retval == -1)
    {
        printf("No se ha podido extraer la IP del cliente.\n");
        res = '3';
        sendMessage(s_local, &res, sizeof(char));
        close(s_local);
        pthread_exit(NULL);
        exit(1);
    }
    char *clt_addr_ip = inet_ntoa(clt_addr.sin_addr);
    strcpy(hostname, clt_addr_ip);
    printf("s> OPERATION FROM %s\n", username);
    // Adquirimos el lock para acceder a la estructura de datos e introducir el nuevo usuario
    pthread_mutex_lock(&m_abb);
    int op_res = connect_user(tree, username, hostname, num);
    print_tree(tree, 1);
    pthread_mutex_unlock(&m_abb);
    switch (op_res)
    {
    case 0:
        res = '0';
        break;
    
    case 1:
        res = '1';
        break;
    
    case 2:
        res = '2';
        break;
    
    case 3:
        res = '3';
        break;
    
    default:
        res = '3';
        break;
    }
    ret = sendMessage(s_local, &res, sizeof(char));
    if (ret < 0)
    {
        res = '2';
        close(s_local);
        pthread_exit(NULL);
        exit(1);
    }

    close(s_local);
    pthread_exit(NULL);
    exit(1);
}

void sigHandler(int signo)
{
    // Handler para controlar la terminación del servidor con un Ctrl+C
    the_end = 1;
}

int main(int argc, char **argv) 
{
    // Se leen los argumentos pasados por consola
    if (argc != 3)
    {
        printf("Uso: %s <hostname del servidor> <puerto servidor>\n", argv[0]);
        printf("Ejemplo -> %s localhost 4200\n", argv[0]);
        return -1;
    }
    if (argv[1] == NULL)
    {
        printf("Hostname no definido\n");
        printf("Ejemplo -> %s localhost 4200\n", argv[0]);
        return -1;
    }
    if (argv[2] == NULL)
    {
        printf("Puerto no definido\n");
        printf("Ejemplo -> %s localhost 4200\n", argv[0]);
        return -1;
    }
    char *endptr;
    long port = strtol(argv[2], &endptr, 10);
    if (*endptr != '\0')
    {
        printf("El valor de Puerto no es un número entero.\n");
        printf("Ejemplo -> %s localhost 4200\n", argv[0]);
        return -1;
    }

    // Definimos el socket
    int sd, sc;
    struct sigaction new_action, old_action;

    char ip[INET_ADDRSTRLEN];
    sd = serverSocket(INADDR_ANY, port, SOCK_STREAM, ip);
    if (sd < 0)
    {
        printf("SERVER: Error al crear el socket\n");
        return -1;
    }
    printf("init server %s:%ld\n", ip, port);

    // Si se presiona Ctrl+C el bucle termina
    new_action.sa_handler = sigHandler;
    sigemptyset(&new_action.sa_mask);
    new_action.sa_flags = 0;
    sigaction(SIGINT, NULL, &old_action);
    if (old_action.sa_handler != SIG_IGN)
    {
        sigaction(SIGINT, &new_action, NULL);
    }

    // Definimos los atributos de los hilos
    pthread_t thid;
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);    // Los hilos se crearén DETACHED ya que 
                                                                    // su ejecución es independiente y no haremos 
                                                                    // un join para esperar su terminación

    // Inicializamos mutex y variables condición
    pthread_mutex_init(&m_msg, NULL);
    pthread_mutex_init(&m_abb, NULL);
    pthread_cond_init(&c_msg, NULL);

    // Inicializamos la estructura de datos (Árbol binario)
    init_tree(&tree);

    while (0 == the_end)
    {
        printf("s> \n");
        bool loopError = false;                                   // Variable de control de errores durante cada iteración
        // Leemos la petición entrante en el socket
        sc = serverAccept(sd);
        if (sc < 0)
        {
            loopError = true;
            continue;
        }

        

        if (loopError == false)
        {
            int ret;                                                    // Variable de control de errores al recibir el mensaje
            char op[MAX_OP_SIZE];                                       // Almacenará la cadena de caracteres mandada 
                                                                        // representando la operación a realizar
                                                                                    
            ret = readLine(sc, (char *)op, MAX_OP_SIZE);
            if (ret < 0)
            {
                loopError = true;
                continue;
            }

            // Operamos con el mensaje recibido
            if (loopError == false)
            {
                if (strcmp(op, "REGISTER") == 0)
                {
                    if (pthread_create(&thid, &attr, (void *)register_svc, (void *)&sc) == 0)
                    {
                        pthread_mutex_lock(&m_msg);
                        while (mensaje_no_copiado)
                        {
                            pthread_cond_wait(&c_msg, &m_msg);
                        }
                        mensaje_no_copiado = true;
                        pthread_mutex_unlock(&m_msg);
                    }
                }else if (strcmp(op, "UNREGISTER") == 0)
                {
                    if (pthread_create(&thid, &attr, (void *)unregister_svc, (void *)&sc) == 0)
                    {
                        pthread_mutex_lock(&m_msg);
                        while (mensaje_no_copiado)
                        {
                            pthread_cond_wait(&c_msg, &m_msg);
                        }
                        mensaje_no_copiado = true;
                        pthread_mutex_unlock(&m_msg);
                    }
                }else if (strcmp(op, "CONNECT") == 0)
                {
                    if (pthread_create(&thid, &attr, (void *)connect_svc, (void *)&sc) == 0)
                    {
                        pthread_mutex_lock(&m_msg);
                        while (mensaje_no_copiado)
                        {
                            pthread_cond_wait(&c_msg, &m_msg);
                        }
                        mensaje_no_copiado = true;
                        pthread_mutex_unlock(&m_msg);
                    }
                }
            }
        }
        loopError = false;
    }

    // Cerramos el socket al finalizar el programa
    closeSocket(sd);

    return 0;
}