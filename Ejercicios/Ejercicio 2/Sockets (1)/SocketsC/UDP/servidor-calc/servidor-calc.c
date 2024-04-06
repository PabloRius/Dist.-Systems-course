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
        int sd;
        int val;
        int32_t peticion[3];
        int32_t res;
	int err;

        if ((sd =  socket(AF_INET, SOCK_DGRAM, 0))<0){
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


	size = sizeof(client_addr);
     	while (1){
    		printf("esperando peticion\n");
		err = recvfrom(sd, (char *) peticion, 3* sizeof(int32_t), 0,
                    	(struct sockaddr *)&client_addr, &size);

		if (err == -1) {
			printf("Error en recvfrom\n");
			return -1;
		}

		printf("Dagagrama recibido de IP: %s   Puerto: %d\n",
				inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

		peticion[0] = ntohl(peticion[0]);  // operacion
     		peticion[1] = ntohl(peticion[1]); 
     		peticion[2] = ntohl(peticion[2]);
     		if (peticion[0]== 0)
          		res = peticion[1] + peticion[2];
     		else
          		res = peticion[1] - peticion[2];


     		res = htonl(res);
     		err = sendto(sd, (char *)&res, sizeof(int32_t), 0, 
					(struct sockaddr *)&client_addr, size);
		if (err == -1) {
			printf("Error en sendto\n");
			return -1;
		}
	}
	close (sd);

     	return(0);
} /*fin main */

