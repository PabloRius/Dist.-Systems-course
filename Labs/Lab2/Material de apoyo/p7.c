#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#define NUM_LECTORES	10
#define NUM_THREADS	11    // 10 lectores y 1 escritor
#define ITER 		40

pthread_mutex_t	mutex_recurso;
pthread_mutex_t	mutex_lectores;
int		num_lectores;
int		recurso = 0;

void lector(void) {
	int j;
	int s;
	double k;

	for(j=0 ; j < ITER; j++) {
		k = (double) rand_r((unsigned int *) &s) / RAND_MAX;	
		usleep((int) 1+(k * 6000000)); // duerme entre 1 y 9 s

		if (pthread_mutex_lock(&mutex_lectores)!=0)
			printf(".......  Error en mutex lock 1 \n");
		num_lectores++;

		if (num_lectores == 1) {
			printf("Primer %lu \n", (unsigned long int) pthread_self());
			pthread_mutex_lock(&mutex_recurso);
		}

		if (pthread_mutex_unlock(&mutex_lectores)!=0)
			printf(".......  Error en mutex unlock 1 \n");

		printf("       Ejecuta el lector %lu lee  %d \n", (unsigned long int) pthread_self(), recurso );

		if (pthread_mutex_lock(&mutex_lectores)!=0)
			printf(".......  Error en mutex lock 2 \n");
		num_lectores--;

		if (num_lectores == 0){
			printf("Ultimo %lu \n", (unsigned long int) pthread_self());
			if( pthread_mutex_unlock(&mutex_recurso) !=0)
				printf("________________ Error en unlock\n");
		}

		if (pthread_mutex_unlock(&mutex_lectores)!=0)
			printf(".......  Error en mutex unlock 2 \n");
	}
	
	pthread_exit(NULL);

}

void escritor(void) {
	int j;
	int s;
	double k;

	for(j=0 ; j < ITER; j++) {
		k = (double) rand_r((unsigned int *) &s) / RAND_MAX;	
		usleep((int) 2 + (k * 5000000)); // duerme entre 2 y 7 s

		pthread_mutex_lock(&mutex_recurso);

		recurso = recurso + 1;
		printf("                                                          Escritor: Nuevo valor %d\n", recurso);

		pthread_mutex_unlock(&mutex_recurso);
	
	}
	
	pthread_exit(NULL);

}

int main(int argc, char *argv[])
{
	int j;
	pthread_attr_t attr;
	pthread_t thid[NUM_THREADS];
	struct timeval t;
	pthread_mutexattr_t   mattr;

	gettimeofday(&t, NULL);
	srand(t.tv_sec);	// se inicializa la semilla de nª pseudoaleatorios

	pthread_attr_init(&attr);

	pthread_mutexattr_init(&mattr);
	pthread_mutexattr_settype(&mattr, PTHREAD_MUTEX_ERRORCHECK);


	pthread_mutex_init(&mutex_recurso, &mattr);
	pthread_mutex_init(&mutex_lectores, &mattr);


	for (j = 0; j < NUM_LECTORES; j++)
		if (pthread_create(&thid[j], NULL, (void *) lector, NULL) == -1){
			printf("Error al crear los threads lectores\n");
			exit(0);
		}
	if (pthread_create(&thid[j], NULL, (void *) escritor, NULL) == -1){
		printf("Error al crear el escritor\n");
		exit(0);
	}

	for (j = 0; j < NUM_THREADS; j++)
		pthread_join(thid[j], NULL);

	exit(0);

}
	

	

