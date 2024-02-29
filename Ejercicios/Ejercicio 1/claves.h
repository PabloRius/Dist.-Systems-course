#ifndef CLAVES_H
#define CLAVES_H 1

#define INIT 0
#define GET 1
#define SET 2
#define MODIFY 3
#define DELETE 4
#define EXIST 5

enum Operation
{
    init = INIT,
    get = GET,
    set = SET,
    modify = MODIFY,
    delet = DELETE, // delete es una keyword reservada
    exist = EXIST
};

typedef struct Message
{
    enum Operation operation;
} Message;

int init_data();

int set_value(int key, char *value1, int N_value2, double *V_value2);

int get_value(int key, char *value1, int *N_value2, double *V_value2);

int modify_value(int key, char *value1, int N_value2, double *V_value2);

int delete_key(int key);

int exist_key(int key);

#endif /* CLAVES_H */