#ifndef UTILS_H
#define UTILS_H 1

void print_double_array(double *array, int size);
void datoc(double *array, int size, char *output);
void ctoda(double *array, int size, char *input);
int conn_socket(char *HOST, int PORT);

#endif /* UTILS_H */