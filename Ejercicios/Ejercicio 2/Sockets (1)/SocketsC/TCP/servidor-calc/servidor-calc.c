#include <stdio.h>
#include <unistd.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "lines.h"

int main(int argc, char *argv[]){
        struct sockaddr_in server_addr,  client_addr;
	socklen_t size;
        int sd, sc;
        int val;
        char op;
        int32_t a, b,  res;
	int err;

        if ((sd =  socket(AF_INET, SOCK_STREAM, 0))<0){
                printf ("SERVER: Error en el socket");
                return (0);
        }
        val = 1;
        setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char *) &val, sizeof(int));

	bzero((char *)&server_addr, sizeof(server_addr));
    	server_addr.sin_family      = AF_INET;
    	server_addr.sin_addr.s_addr = INADDR_ANY;
    	server_addr.sin_port        = htons(4200);

    	err = bind(sd, (const struct sockaddr *)&server_addr,
			sizeof(server_addr));
	if (err == -1) {
		printf("Error en bind\n");
		return -1;
	}

    	err = listen(sd, SOMAXCONN);
	if (err == -1) {
		printf("Error en listen\n");
		return -1;
	}

    	size = sizeof(client_addr);

     	while (1){
    		printf("esperando conexion\n");
    		sc = accept(sd, (struct sockaddr *)&client_addr, (socklen_t *)&size);

		if (sc == -1) {
			printf("Error en accept\n");
			return -1;
		}
		printf("conexi贸n aceptada de IP: %s   Puerto: %d\n",
				inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

  		err = recvMessage ( sc, (char *) &op, sizeof(char));   // recibe la operaci贸
		if (err == -1) {
			printf("Error en recepcion\n");
			close(sc);
			continue;
		}
		err = recvMessage( sc, (char *) &a, sizeof(int32_t)); // recibe a
		if (err == -1) {
			printf("Error en recepcion\n");
			close(sc);
			continue;
		}
		err = recvMessage( sc, (char *) &b, sizeof(int32_t)); // recibe b
		if (err == -1) {
			printf("Error en recepcion\n");
			close(sc);
			continue;
		}
		a = ntohl(a);
		b = ntohl(b);

   		if (op == 0) // procesa la petici贸n
      			res = a + b;
   		else
      			res = a - b;

		res = htonl(res);
   		err = sendMessage(sc, (char *)&res, sizeof(int32_t));  // env铆a el resultado
		if (err == -1) {
			printf("Error en envi璷\n");
			close(sc);
			continue;
		}

   		close(sc);                      // cierra la conexi贸n (sc)
	}
	close (sd);

     	return(0);
} /*fin main */

