#include <stdio.h>
#include <netdb.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "lines.h"

int  main(int argc, char **argv)  // en argv[1] == servidor 
{
        int sd;
        struct sockaddr_in server_addr;
        struct hostent *hp;
        int32_t peticion[3], res;
	int err;

        if (argc != 2){
                printf("Uso: cliente <direccion_servidor> \n");
                return(0);
        }

        sd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sd == 1) {
		printf("Error en socket\n");
		return -1;
	}


	bzero((char *)&server_addr, sizeof(server_addr));
   	hp = gethostbyname (argv[1]);
	if (hp == NULL) {
		printf("Error en gethostbyname\n");
		return -1;
	}

   	memcpy (&(server_addr.sin_addr), hp->h_addr, hp->h_length);
   	server_addr.sin_family  = AF_INET;
   	server_addr.sin_port    = htons(4200);
   	
	peticion[0] = htonl(0);         // sumar
     	peticion[1] = htonl(5);
     	peticion[2] = htonl(2);

	err = sendto(sd, (char *) peticion, 3 * sizeof(int32_t), 0,
            (struct sockaddr *) &server_addr, sizeof(server_addr));
	if (err == 1) {
		printf("Error en sendto\n");
		return -1;
	}

     	err = recvfrom(sd, (char *)&res, sizeof(int32_t), 0, NULL, NULL);
	if (err == 1) {
		printf("Error en recvfrom\n");
		return -1;
	}

     	res = ntohl(res);
	
    	printf("Resultado es %d \n", res);

   	close (sd);
   	return(0);
} /* fin main */
