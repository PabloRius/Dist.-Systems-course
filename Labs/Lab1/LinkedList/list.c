#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include "list.h"


int init(List *l) {

	*l = NULL;
	return (0);
}	

int set(List *l, char *key, int value){
	struct Node *ptr;

	ptr = (struct Node *) malloc(sizeof(struct Node));
	if (ptr == NULL) 
		return -1;

	if (*l == NULL) {  // emtpy list
		strcpy(ptr->key, key);
		ptr->value = value;
		ptr->next = NULL;
		*l = ptr;
	}
	else {
		// insert in head
		strcpy(ptr->key, key);
		ptr->value = value;
		ptr->next = *l;
		*l = ptr;
	}


	return 0;
}	

int get(List l, char *key, int *value){
	List aux;

	aux = l;	

	while (aux!=NULL) {
		if (strcmp(aux->key, key)==0) {
			*value = aux->value;
			return 0;		// found
		}
		else
			aux = aux->next;
	}

	return -1;  // not found
}	

int printList(List l){
	List aux;

	aux = l;

	while(aux != NULL){
		printf("Key=%s    value=%d\n", aux->key, aux->value);
		aux = aux->next;
	}
	return 0;
}	

int delete(List *l, char *key){
	List aux, back;

	if (*l == NULL)  // lista vacia
		return 0;

	// primer elemento de la lista
	if (strcmp(key, (*l)->key) == 0){
		aux = *l;
		*l = (*l)->next;
		free(aux);
		return 0;
	}
	
	aux = *l;
	back = *l;
	while (aux!=NULL) {
		if (strcmp(aux->key, key)==0) {
			back->next = aux->next;
			free (aux);
			return 0;		// found
		}
		else {
			back = aux;
			aux = aux->next;
		}
	}

	return 0;
}	

int  destroy(List *l){
	List aux; 

	while (*l != NULL){
		aux = *l;
		*l = aux->next;
		free(aux);
	}

	return 0;
}	

