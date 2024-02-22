#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

#define N 1E8
#define D 1E-8

#define SIZE 20

double PI;

pthread_mutex_t m;
pthread_cond_t c;
int copia = 0;

pthread_mutex_t m_pi;

void calcula(int *d)
{
    double result = 0.0, x;
    int mid;

    pthread_mutex_lock(&m);
    mid = *d;
    copia = 1;
    pthread_cond_signal(&c);
    pthread_mutex_unlock(&m);

    result = 0;
    for (int j = mid; j < N; j += SIZE)
    {
        x = D * j;
        result += sqrt(4 * (1 - x * x));
    }

    pthread_mutex_lock(&m_pi);
    PI += D * 2 * result;
    pthread_mutex_unlock(&m_pi);

    return;
}

int main(int argc, char *argv[])
{
    int i;
    struct timeval t1, t2;
    long t;

    pthread_t thid[SIZE];
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_mutex_init(&m, NULL);
    pthread_mutex_init(&m_pi, NULL);
    pthread_cond_init(&c, NULL);

    gettimeofday(&t1, 0);

    for (i = 0; i < SIZE; i++)
    {
        if (pthread_create(&thid[i], NULL, (void *)calcula, &i) == -1)
        {
            printf("Error al crear los threads\n");
            exit(0);
        }

        pthread_mutex_lock(&m);
        while (copia == 0)
        {
            pthread_cond_wait(&c, &m);
        }

        copia = 0;
        pthread_mutex_unlock(&m);
    }

    for (int j = 0; j < SIZE; j++)
        pthread_join(thid[j], NULL);

    gettimeofday(&t2, 0);
    t = (t2.tv_sec - t1.tv_sec) * 1000000 + t2.tv_usec - t1.tv_usec;
    printf("El tiempo es %f ms\n", (t / 1000.0));

    printf("PI = %.9f\n", PI);

    return 0;
}