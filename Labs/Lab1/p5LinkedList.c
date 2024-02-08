#include <stdio.h>
#include <stdlib.h>
#include "p5LinkedList.h"

// Funcion para inicializar la lista enlazada
struct LinkedList * init(){
    struct LinkedList *linked_list = (struct LinkedList *)malloc(sizeof(struct LinkedList));
    linked_list->head = NULL;
    linked_list->tail = NULL;
    linked_list->length = 0;
    return linked_list;
}

// Funcion para meter un nodo nuevo al inicio de la lista enlazada
void head(struct LinkedList *list, char *str_value, int int_value){
    struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));
    if(new_node == NULL){
        printf("Error al asignar el espacio al nuevo nodo");
        exit(-1);
    }
    new_node->str_value = str_value;
    new_node->int_value = int_value;
    if(list->length == 0){
        list->head = new_node;
        list->tail = new_node;
        
    }else{
        new_node->next = list->head;
        list->head = new_node;
    }
    list->length += 1;

}

// Funcion para meter un nodo nuevo a la cola de la lista enlazada
void push(struct LinkedList *list, char *str_value, int int_value){
    struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));
    if(new_node == NULL){
        printf("Error al asignar el espacio al nuevo nodo");
        exit(-1);
    }
    new_node->str_value = str_value;
    new_node->int_value = int_value;
    if(list->length == 0){
        list->head = new_node;
        list->tail = new_node;
        
    }else{
        list->tail->next = new_node;
        list->tail = new_node;
    }
    list->length += 1;
}

// Funcion para imprimir el contenido de los nodos de una lista enlazada
void print(struct LinkedList *list){
    struct Node *current_node = list->head;
    if(current_node == NULL){
        printf("La lista está vacía\n");
    }else{
        printf("Inicio de la lista enlazada\n");
        while(current_node != NULL){
            printf("Valor entero: %d, Valor en string: %s\n",current_node->int_value,current_node->str_value);
            current_node = current_node->next;
        }
        printf("Fin de la lista enlazada\n");
    }
    
}