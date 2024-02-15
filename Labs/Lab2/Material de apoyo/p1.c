#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#define NUM_THREADS	2
#define ITER 		10

void funcion(int *id) {
	int j;
	int s;
	double k;
	int mid = *id;  // cada thread recibe un número (0 o 1)

	for(j=0 ; j < ITER; j++) {
		k = (double) rand_r((unsigned int *) &s) / RAND_MAX;	
		usleep((int) (k * 100000)); // duerme entre 0 y 100 ms
		printf("Ejecuta el thread %d iteracion %d \n", mid, j );
	}
	
	pthread_exit(NULL);

}

int main(int argc, char *argv[])
{
	int j;
	pthread_attr_t attr;
	pthread_t thid[NUM_THREADS];
	struct timeval t;

	gettimeofday(&t, NULL);
	srand(t.tv_sec);	// se inicializa la semilla de nª pseudoaleatorios

	pthread_attr_init(&attr);

	for (j = 0; j < NUM_THREADS; j++)
		if (pthread_create(&thid[j], NULL, (void *) funcion, &j) == -1){
			printf("Error al crear los threads\n");
			exit(0);
		}

	for (j = 0; j < NUM_THREADS; j++)
		pthread_join(thid[j], NULL);

	exit(0);

}
	

	

