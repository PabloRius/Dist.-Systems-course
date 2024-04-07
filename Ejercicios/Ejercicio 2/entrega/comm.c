#include "comm.h"

int serverSocket(unsigned int addr, int port, int type)
{
    struct sockaddr_in server_addr;
    int sd, ret;

    // Crear socket
    sd = socket(AF_INET, type, 0);
    if (sd < 0)
    {
        perror("socket: ");
        return (0);
    }

    // Opción de reusar dirección
    int val = 1;
    setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char *)&val, sizeof(int));

    // Dirección
    bzero((char *)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    // Bind
    ret = bind(sd, (const struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret == -1)
    {
        perror("bind: ");
        return -1;
    }

    // Listen
    ret = listen(sd, SOMAXCONN);
    if (ret == -1)
    {
        perror("listen: ");
        return -1;
    }

    return sd;
}

int serverAccept(int sd)
{
    int sc;
    struct sockaddr_in client_addr;
    socklen_t size;

    printf("esperando conexión...\n");

    size = sizeof(client_addr);
    sc = accept(sd, (struct sockaddr *)&client_addr, (socklen_t *)&size);
    if (sc < 0)
    {
        perror("accept: ");
        return -1;
    }

    printf("conexión aceptada de IP: %s y puerto: %d\n",
           inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

    return sc;
}

int clientSocket(char *remote, int port)
{
    struct sockaddr_in server_addr;
    struct hostent *hp;
    int sd, ret;

    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd < 0)
    {
        perror("socket: ");
        return -1;
    }

    hp = gethostbyname(remote);
    if (hp == NULL)
    {
        printf("Error en gethostbyname\n");
        return -1;
    }

    bzero((char *)&server_addr, sizeof(server_addr));
    memcpy(&(server_addr.sin_addr), hp->h_addr, hp->h_length);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    ret = connect(sd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret < 0)
    {
        perror("connect: ");
        return -1;
    }

    return sd;
}

int closeSocket(int sd)
{
    int ret;

    ret = close(sd);
    if (ret < 0)
    {
        perror("close: ");
        return -1;
    }

    return ret;
}

int sendMessage(int socket, char *buffer, int len)
{
    int r;
    int l = len;

    do
    {
        r = write(socket, buffer, l);
        if (r < 0)
        {
            return (-1); /* error */
        }
        l = l - r;
        buffer = buffer + r;

    } while ((l > 0) && (r >= 0));

    return 0;
}

int recvMessage(int socket, char *buffer, int len)
{
    int r;
    int l = len;

    do
    {
        r = read(socket, buffer, l);
        if (r < 0)
        {
            return (-1); /* error */
        }
        l = l - r;
        buffer = buffer + r;

    } while ((l > 0) && (r >= 0));

    return 0;
}

ssize_t writeLine(int fd, char *buffer)
{
    return sendMessage(fd, buffer, strlen(buffer) + 1);
}

ssize_t readLine(int fd, char *buffer, size_t n)
{
    ssize_t numRead; /* bytes leídos en último read() */
    size_t totRead;  /* bytes leídos hasta ahora */
    char *buf;
    char ch;

    if (n <= 0 || buffer == NULL)
    {
        errno = EINVAL;
        return -1;
    }

    buf = buffer;
    totRead = 0;

    while (1)
    {
        numRead = read(fd, &ch, 1); /* leer un byte */

        if (numRead == -1)
        {
            if (errno == EINTR) /* interrupción -> reiniciar read() */
                continue;
            else
                return -1; /* otro tipo de error */
        }
        else if (numRead == 0)
        {                     /* EOF */
            if (totRead == 0) /* no bytes leídos -> return 0 */
                return 0;
            else
                break;
        }
        else
        { /* numRead debe ser 1 aquí */
            if (ch == '\n')
                break;
            if (ch == '\0')
                break;
            if (totRead < n - 1)
            { /* descartar > (n-1) bytes */
                totRead++;
                *buf++ = ch;
            }
        }
    }

    *buf = '\0';
    return totRead;
}