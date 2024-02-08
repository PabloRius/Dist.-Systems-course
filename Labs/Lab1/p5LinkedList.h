#ifndef _LISTA_H
#define _LISTA_H        1

// Defino la estructura de un nodo de la lista
struct Node {
    char *str_value;            // El valor en string
    int int_value;              // El valor en entero
    struct Node *next;          // Apuntará al siguiente nodo de la lista
};

// Defino la estructura de la lista, con cabeza, cola y longitud
struct LinkedList {
    struct Node *head;          // El primer nodo de la lista
    struct Node *tail;          // El último nodo de la lista
    int length;                 // La longitud de la lista
};

struct LinkedList * init();
void head(struct LinkedList *list, char *str_value, int int_value);
void push(struct LinkedList *list, char *str_value, int int_value);
void print(struct LinkedList *list);

#endif