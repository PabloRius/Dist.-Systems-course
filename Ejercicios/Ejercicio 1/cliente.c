#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>

#define MAX_BUFFER 10           /* tamaño del buffer */
#define DATOS_A_PRODUCIR 100000 /* datos a producir */

int main()
{

    mqd_t mq; /* cola de mensajes donde dejar los
        datos producidos y extraer los datos a consumir */

    mq = mq_open("/tuple_sv_queue", O_RDWR);
    if (mq == -1)
    {
        perror("mq_open");
        exit(-1);
    }

    char *dato = malloc(sizeof(char)); // Asignar memoria para dato
    if (dato == NULL)
    {
        perror("malloc");
        exit(-1);
    }
    int i;
    char consumableData[10] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j'};

    for (i = 0; i < 10; i++)
    {
        /* producir dato */
        *dato = consumableData[i];
        printf("Sending data: %s\n", dato);
        if (mq_send(mq, (char *)dato, sizeof(char), 0) == -1)
        {
            perror("mq_send");
            mq_close(mq);
            exit(1);
        }
    } /* end for */

    mq_close(mq);
    return (0);
}
