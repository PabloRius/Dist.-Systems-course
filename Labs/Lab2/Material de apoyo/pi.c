#include <stdio.h>
#include <math.h>
#include <stdio.h>
#include <sys/time.h>

#define N 1E7
#define d 1E-7

int main (int argc, char* argv[])
{
    int i;
    double pi=0.0, result=0.0, sum=0.0, x;
    struct timeval t1, t2;
    long t;


    gettimeofday(&t1, 0);

    result = 0;
    for (i=0; i<N; i++) {
        x = d *i;
        result+=sqrt(4*(1-x*x));

    }

    gettimeofday(&t2, 0);
    t = (t2.tv_sec-t1.tv_sec)*1000000 + t2.tv_usec-t1.tv_usec;
    printf("El tiempo es %f ms\n", (t / 1000.0));

    pi=d*2*result;
    printf("PI=%.9f\n", pi);

    return 0;
}