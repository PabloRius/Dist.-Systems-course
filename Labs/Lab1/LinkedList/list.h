#ifndef _LISTA_H
#define _LISTA_H        1

#define MAX_KEY_LENGTH	256

struct Node{ 
	char 	key[MAX_KEY_LENGTH];
	int 	value;
	struct 	Node *next; 
};


typedef struct Node * List;


int init(List *l);
int set(List *l, char *key,  int value);
int get(List l, char *key,  int *value);
int printList(List l);
int delete(List *l, char *key);
int destroy(List *l);

#endif

