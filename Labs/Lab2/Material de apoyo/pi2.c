#include <stdio.h>
#include <math.h>
#include <stdio.h>
#include <sys/time.h>
#include <pthread.h>

#define N 1E7
#define d 1E-7

#define SIZE 20

double PI;

void calcula(int id)
{
    double result = 0.0, x;
    int i;

    result = 0;
    for (i = id; i < N; i = i + SIZE)
    {
        x = d * i;
        result += sqrt(4 * (1 - x * x));
    }

    PI = PI + d * 2 * result;
    return;
}

int main(int argc, char *argv[])
{
    int i;
    struct timeval t1, t2;
    long t;

    pthread_t thid[SIZE];
    pthread_attr_t attr;
    pthread_mutex_t m;

    pthread_attr_init(&attr);
    pthread_mutex_init(&m, NULL);

    gettimeofday(&t1, 0);

    for (i = 0; i < SIZE; i++)
    {
        pthread_create(&thid[i], NULL, (void *)calcula, i);
    }

    for (j = 0; j < SIZE; j++)
        pthread_join(thid[j], NULL);

    gettimeofday(&t2, 0);
    t = (t2.tv_sec - t1.tv_sec) * 1000000 + t2.tv_usec - t1.tv_usec;
    printf("El tiempo es %f ms\n", (t / 1000.0));

    printf("PI = %.9f\n", PI);

    return 0;
}