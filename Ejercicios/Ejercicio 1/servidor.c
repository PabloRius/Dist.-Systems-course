#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>

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

void init(struct Mensaje msg)
{
    printf("Init service, response queue: %s\n", msg.queue);

    struct Respuesta res;
    res.codigo = 1;
    mqd_t mq;
    mq = mq_open(msg.queue, O_WRONLY);
    if (mq == -1)
    {
        perror("mq_open");
        exit(-1);
    }
    if (mq_send(mq, (const char *)&res, sizeof(res), 0) < 0)
    {
        perror("mq_send");
        mq_close(mq);
        exit(1);
    }
}

void get_tuple(struct Mensaje msg)
{
    printf("Get tuple service + %s\n", msg.cadena);
}

void set_tuple(struct Mensaje msg)
{
    printf("Set tuple service + %s\n", msg.cadena);
}

void modify_tuple(struct Mensaje msg)
{
    printf("Modify tuple service + %s\n", msg.cadena);
}

void delete_tuple(struct Mensaje msg)
{
    printf("Delete tuple service + %s\n", msg.cadena);
}

void exist_tuple(struct Mensaje msg)
{
    printf("Exist tuple service + %s\n", msg.cadena);
}

int main(int argc, char **argv)
{

    mqd_t mq; /* cola de mensajes donde dejar los    datos
  producidos y extraer los datos a consumir */
    struct mq_attr attr;

    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(struct Mensaje);
    mq = mq_open("/tuple_sv_queue", O_CREAT | O_RDWR, 0700, &attr);
    if (mq == -1)
    {
        perror("mq_open");
        exit(-1);
    }

    struct Mensaje msg;

    for (;;)
    {
        /* recibir dato */
        printf("Esperando un dato\n");
        if (mq_receive(mq, (char *)&msg, sizeof(msg), 0) == -1)
        {
            perror("mq_receive");
            mq_close(mq);
            exit(1);
        }
        /* Consumir el dato */
        char *end;

        switch (strtol(msg.op, &end, 10))
        {
        case INIT:
            init(msg);
            break;

        case GET:
            get_tuple(msg);
            break;

        case SET:
            set_tuple(msg);
            break;

        case MODIFY:
            modify_tuple(msg);
            break;

        case DELETE:
            delete_tuple(msg);
            break;

        case EXIST:
            exist_tuple(msg);
            break;

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
    } /* end for */

    mq_close(mq);

    return (0);
}