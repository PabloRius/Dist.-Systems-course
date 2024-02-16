#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#define NUM_THREADS 10
#define ITER 2

pthread_mutex_t m;
pthread_mutex_t cm;
pthread_cond_t c;
pthread_cond_t altern;

int copia = 1;
int current_thread = 0;

void funcion(int *id)
{
    int j;
    int s;
    double k;
    pthread_mutex_lock(&m);
    int mid = *id; // cada thread recibe un número (0 o 1)
    copia = 1;
    pthread_cond_signal(&c);
    pthread_mutex_unlock(&m);
    while (1)
    {
        pthread_mutex_lock(&cm);
        while (current_thread != mid)
        {
            pthread_cond_wait(&altern, &cm);
        }
        for (j = 0; j < ITER; j++)
        {
            k = (double)rand_r((unsigned int *)&s) / RAND_MAX;
            usleep((int)(k * 100000)); // duerme entre 0 y 100 ms
            printf("Ejecuta el thread %d iteracion %d \n", mid, j);
        }
        current_thread += 1;
        if (current_thread == NUM_THREADS)
        {
            current_thread = 0;
        }
        pthread_cond_signal(&altern);
        pthread_mutex_unlock(&cm);
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    int j;
    pthread_attr_t attr;
    pthread_t thid[NUM_THREADS];
    struct timeval t;

    pthread_mutex_init(&m, NULL);
    pthread_cond_init(&c, NULL);

    gettimeofday(&t, NULL);
    srand(t.tv_sec); // se inicializa la semilla de nª pseudoaleatorios

    pthread_attr_init(&attr);

    for (j = 0; j < NUM_THREADS; j++)
    {
        pthread_mutex_lock(&m);
        while (copia == 0)
        {
            pthread_cond_wait(&c, &m);
        }
        pthread_create(&thid[j], NULL, (void *)funcion, &j);
        copia = 0;
        pthread_mutex_unlock(&m);
    }

    for (j = 0; j < NUM_THREADS; j++)
        pthread_join(thid[j], NULL);

    exit(0);
}
