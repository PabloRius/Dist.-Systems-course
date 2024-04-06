#include "claves.h"
#include "mensaje.h"
#include "comm.h"
#include <stdio.h>
#include <string.h>

#define MAX_LENGTH 255

#define INIT "0"
#define GET "1"
#define SET "2"
#define MODIFY "3"
#define DELETE "4"
#define EXIST "5"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 4200


int init()
{
    int sock;
    char buffer[1024];
    int len = 256;
    int ret;

    // Establecer conexión con el servidor.
    sock = clientSocket(SERVER_IP, SERVER_PORT);
    if (sock < 0) {
        fprintf(stderr, "init: Error al crear el socket cliente\n");
        return -1;
    }

    // Enviar comando de inicialización al servidor.
    strcpy(buffer, "INIT");
    ret = sendMessage(sock, buffer, strlen(buffer) + 1);
    if (ret < 0) {
        fprintf(stderr, "init: Error al enviar mensaje al servidor\n");

        return -1;
    }

    // Esperar respuesta del servidor.
    memset(buffer, 0, len); // Limpiar el buffer antes de recibir el mensaje.
    ret = recvMessage(sock, buffer, len);
    if (ret <= 0) {
        fprintf(stderr, "init: Error al recibir mensaje del servidor\n");

        return -1;
    }

    // Interpretar respuesta. no se como has hecho la repsuesta del server, si devuelvo OK todo bien
    if (strcmp(buffer, "OK") == 0) {
        printf("init: Inicialización completada con éxito\n");

        return 0;
    } else {
        fprintf(stderr, "init: Error durante la inicialización\n");

        return -1;
    }
}

int set_value(int key, char *value1, int N_value2, double *V_value2)
{
    int sock, ret;
    char buffer[1024];
    int buffer_len = 1024, pos = 0;

    // Establecer conexión con el servidor.
    sock = clientSocket(SERVER_IP, SERVER_PORT);
    if (sock < 0) {
        fprintf(stderr, "set_value: Error al crear el socket cliente\n");
        return -1;
    }

    // Preparar y enviar el mensaje al servidor
    pos += snprintf(buffer + pos, buffer_len - pos, "SET %d %s %d ", key, value1, N_value2);
    for (int i = 0; i < N_value2; ++i) {
        pos += snprintf(buffer + pos, buffer_len - pos, "%lf ", V_value2[i]);
    }

    ret = sendMessage(sock, buffer, pos); // pos es ahora la longitud del mensaje.
    if (ret < 0) {
        fprintf(stderr, "set_value: Error al enviar mensaje al servidor\n");

        return -1;
    }

    // Esperar respuesta del servidor.
    memset(buffer, 0, buffer_len); // Limpiar el buffer antes de recibir el mensaje.
    ret = recvMessage(sock, buffer, buffer_len);
    if (ret <= 0) {
        fprintf(stderr, "set_value: Error al recibir mensaje del servidor\n");

        return -1;
    }

    // Interpretar respuesta.
    if (strcmp(buffer, "OK") == 0) {
        printf("set_value: Tupla insertada con éxito\n");

        return 0; // Éxito
    } else {
        fprintf(stderr, "set_value: Error al insertar tupla\n");
        return -1; // Fallo
    }
}

int get_value(int key, char *value1, int *N_value2, double *V_value2)
{
    char sendBuffer[1024];
    char recvBuffer[1024];
    int sock, ret;


    // Establecer conexión con el servidor.
    sock = clientSocket(SERVER_IP, SERVER_PORT);
    if (sock < 0) {
        fprintf(stderr, "Error al crear el socket cliente\n");
        return -1;
    }


    // Preparar mensaje de solicitud
    snprintf(sendBuffer, sizeof(sendBuffer), "GET %d", key);
    ret = sendMessage(sock, sendBuffer, strlen(sendBuffer) + 1); // +1 para incluir el terminador nulo
    if (ret < 0) {
        fprintf(stderr, "Error al enviar mensaje al servidor\n");
        return -1;
    }


    // Recibir y procesar la respuesta del servidor
    ret = recvMessage(sock, recvBuffer, sizeof(recvBuffer));
    if (ret <= 0) {
        fprintf(stderr, "Error al recibir mensaje del servidor\n");
        return -1;
    }

    // Procesar la respuesta. Suponiendo que la respuesta comienza con un código de éxito o error.
    int codigo;
    sscanf(recvBuffer, "%d", &codigo);
    if (codigo != 0) { // Supongamos que 0 es éxito
        fprintf(stderr, "Error en la respuesta del servidor\n");
        return -1;
    }

    return 0; // Éxito
}

int modify_value(int key, char *value1, int N_value2, double *V_value2)
{
    char sendBuffer[1024];
    char recvBuffer[1024];
    int sock, ret;


    // Establecer conexión con el servidor.
    sock = clientSocket(SERVER_IP, SERVER_PORT);
    if (sock < 0) {
        fprintf(stderr, "Error al crear el socket cliente\n");
        return -1;
    }

    // Preparar mensaje de solicitud.
    int offset = snprintf(sendBuffer, sizeof(sendBuffer), "MODIFY %d %s %d", key, value1, N_value2);
    for (int i = 0; i < N_value2 && offset < sizeof(sendBuffer); ++i) {
        offset += snprintf(sendBuffer + offset, sizeof(sendBuffer) - offset, " %lf", V_value2[i]);
    }


    ret = sendMessage(sock, sendBuffer, strlen(sendBuffer) + 1); // +1 para incluir el terminador nulo
    if (ret < 0) {
        fprintf(stderr, "Error al enviar mensaje al servidor\n");
        return -1;
    }


    // Recibir y procesar la respuesta del servidor.
    ret = recvMessage(sock, recvBuffer, sizeof(recvBuffer));
    if (ret <= 0) {
        fprintf(stderr, "Error al recibir mensaje del servidor\n");
        return -1;
    }



    int codigo;
    sscanf(recvBuffer, "%d", &codigo);
    if (codigo != 0) { // Supongamos que 0 es éxito.
        fprintf(stderr, "Error en la respuesta del servidor\n");
        return -1;
    }

    return 0; // Éxito
}

int delete_key(int key)
{
    char sendBuffer[1024]; // Asumiendo que este tamaño es suficiente para la solicitud
    char recvBuffer[1024]; // Asumiendo que este tamaño es suficiente para la respuesta
    int sock, ret;


    // Establecer conexión con el servidor.
    sock = clientSocket(SERVER_IP, SERVER_PORT);
    if (sock < 0) {
        fprintf(stderr, "Error al crear el socket cliente\n");
        return -1;
    }


    // Preparar mensaje de solicitud.
    snprintf(sendBuffer, sizeof(sendBuffer), "DELETE %d", key);


    ret = sendMessage(sock, sendBuffer, strlen(sendBuffer) + 1); // +1 para incluir el terminador nulo
    if (ret < 0) {
        fprintf(stderr, "Error al enviar mensaje al servidor\n");
        return -1;
    }


    // Recibir y procesar la respuesta del servidor.
    ret = recvMessage(sock, recvBuffer, sizeof(recvBuffer));
    if (ret <= 0) {
        fprintf(stderr, "Error al recibir mensaje del servidor\n");
        return -1;
    }


    // Procesar la respuesta. Supongamos que la respuesta es un simple código de éxito/error.
    int codigo;
    sscanf(recvBuffer, "%d", &codigo);
    if (codigo != 0) { // Suponiendo que 0 es éxito.
        fprintf(stderr, "Error en la respuesta del servidor\n");
        return -1;
    }

    return 0; // Éxito
}


int exist(int key)
{
    char sendBuffer[256]; // Asumiendo que este tamaño es suficiente para la solicitud
    char recvBuffer[256]; // Asumiendo que este tamaño es suficiente para la respuesta
    int sock, ret;


    // Establecer conexión con el servidor.
    sock = clientSocket(SERVER_IP, SERVER_PORT);
    if (sock < 0) {
        fprintf(stderr, "Error al crear el socket cliente\n");
        return -1;
    }


    // Preparar mensaje de solicitud. Formato esperado: "E key", donde "E" indica operación exist
    snprintf(sendBuffer, sizeof(sendBuffer), "E %d", key);

    ret = sendMessage(sock, sendBuffer, strlen(sendBuffer) + 1); // +1 para incluir el terminador nulo
    if (ret < 0) {
        fprintf(stderr, "Error al enviar mensaje al servidor\n");
        return -1;
    }


    // Recibir y procesar la respuesta del servidor.
    ret = recvMessage(sock, recvBuffer, sizeof(recvBuffer));
    if (ret <= 0) {
        fprintf(stderr, "Error al recibir mensaje del servidor\n");
        return -1;
    }


    // Procesar la respuesta.
    int existe;
    sscanf(recvBuffer, "%d", &existe);



    if (existe == 1) {
        return 1; // La tupla existe
    } else if (existe == 0) {
        return 0; // La tupla no existe
    } else {
        fprintf(stderr, "Error en la respuesta del servidor\n");
        return -1; // Error en la operación
    }
}
