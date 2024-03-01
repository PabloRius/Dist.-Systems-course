#define MAX_MESSAGE 255

struct Mensaje{
    char op[1];
    int key;
    char cadena[MAX_MESSAGE];
    int N;
    double *vector;
};