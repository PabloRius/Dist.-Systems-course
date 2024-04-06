#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
int main(int argc, char **argv) {
           struct hostent *hp;
           struct in_addr in;

	   if (argc < 2) {
		   printf("Uso: %s <address>\n", argv[0]);
		   return 0;
	   }

           hp = gethostbyname(argv[1]);
           if (hp == NULL) {
              printf("Error en gethostbyname\n");     exit(0);
           }

           memcpy(&in.s_addr,*(hp->h_addr_list),sizeof(in.s_addr));
           printf("%s es %s\n", hp->h_name, inet_ntoa(in));
           
	   return 0;
}
