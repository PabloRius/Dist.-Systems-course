#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <pthread.h>

#define BUF_SIZE	256

char buffer[BUF_SIZE];
int  n_elementos = 0;

void productor(int *f) {
	char c;
	int pos = 0;

	while (read (*f, &c, 1) >0) {
		buffer[pos] = c;
		pos = (pos + 1) % BUF_SIZE;
	}
	
	pthread_exit(NULL);

}

void consumidor(int *f){
	char c;
	int pos = 0;

	for(;;) {
		c = buffer[pos];
		pos = (pos + 1) % BUF_SIZE;

		
		if (write(*f, &c, 1) < 0) {
			printf("Error de escritura \n");
			exit(0);
		}
	}

	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	pthread_attr_t attr;
	pthread_t thid[2];
	int fe, fs;


	if (argc < 3) {
		printf("Uso: %s  <fichero entrada> <fichero salida> \n", argv[0]);
		exit(0);
	}

	fe = open (argv[1], O_RDONLY);
	if (fe < 0) {
		printf("Error al abrir %s\n", argv[1]);
		exit(0);
	}

	fs = creat (argv[2], 0700);
	if (fs < 0) {
		printf("Error al crear %s\n", argv[2]);
		close(fe);
		exit(0);
	}

	pthread_attr_init(&attr);

	if (pthread_create(&thid[0], NULL, (void *) productor, &fe) == -1){
		printf("Error al crear el productor\n");
		exit(0);
	}

	if (pthread_create(&thid[1], NULL, (void *) consumidor, &fs) == -1){
		printf("Error al crear el consumidor\n");
		exit(0);
	}

	pthread_join(thid[0], NULL);
	pthread_join(thid[1], NULL);

	close(fe);
	close(fs);

	exit(0);

}
	

	

