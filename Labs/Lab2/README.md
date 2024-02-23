# Laboratorio 1 - Concurrencia

## Ejercicio 1

Disponemos de un código inicial (p1.c)

```c
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#define NUM_THREADS     2
#define ITER            10

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
        srand(t.tv_sec);        // se inicializa la semilla de nº pseudoaleatorios

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
```

Este programa crea dos procesos ligeros, cada uno se itera bloquenadose un tiempo aleatorio en cada iteración (entre 0 y 100ms) e imprime su id y el número de iteración.
Identifique los problemas que se observan al compilar y ejecutar el código.

## Ejercicio 2

Modifique el programa anterior (p2.c) de forma que cada thread imprima su id de forma correcta

## Ejercicio 3

Modifique el programa enterior (p3.c) de forma que el siguiente código se ejecute en cada thread en exclusión mutua, usando un mutex.
Con esta solución podremos acceder a recursos protegidos.
(Para este código de ejemplo, esta solución no sería necesaria ya que no se dan condiciones de carrera, pero es necesario aprender a aplicarla para las aplicaciones en las que sí lo sea).

```c
k = (double) rand_r(&s) / RAND_MAX;
              usleep((int) (k * 100000)); // duerme entre 0 y 100 ms
              printf("Ejecuta el thread %d iteracion %d \n", mid, j );
```

¿Qué problemas observa?

## Ejercicio 4

Modifique p2.c (p4.c) de forma que los threads ejecuten el siguiente código

```c
k = (double) rand_r(&s) / RAND_MAX;
              usleep((int) (k * 100000)); // duerme entre 0 y 100 ms
              printf("Ejecuta el thread %d iteracion %d \n", mid, j );
```

de forma alternada, primero el thread 0 y luego el 1.

## Ejercicio 5

Modifique el programa anterior (p5.c) de forma que se creen 10 threads que ejecuten el código destacado de forma ordenada, primero el 0, luego el 1, hasta el 9 y luego de nuevo el 0, etc.

## Ejercicio 6

En el material de apoyo se ha proporcionado pi.c para calcular el valor de pi.
pi2.c es una versión similar pensada para primeramente parelelizar el código para que se pueda ejecutar en varios threads. Compruebe el tiempo de ejecución en cada versión.

## Ejercicio 7

En el material de apoyo se proporciona el código de un productor-consumidor (p6.c), que copia un archivo pasado como parámetro en otro. El productor lee el fichero e inserta caracteres en un buffer, el consumidor lee caracteres del buffer y los escribe en el fichero de salida. Modifique este programa (p7.c) para que la ejecución de ambos procesos sea la correcta (el proceso productor debe notificar al proceso consumidor cuando ha terminado la lectura).

## Ejercicio 8

En el material de apoyo se proporciona el código de un programa (p7.c) que intenta resolver el problema de los lectores-escritores (dando prioridad a los lectores). Ejecute el programa e identifique el problema.
Modifique el programa (p8.c) para que la ejecución sea la correcta.

## Ejercicio 9

Resuelva todos los problemas anteriores en Python.
