#include <stdio.h>
#include <stdlib.h>
#include "p5LinkedList.h"

void readArgs(int argc, char **argv, struct LinkedList *list){
    // Ya que las cadenas de caracteres tienen un tamaño indeterminado, 
    // el array contendrá punteros a estas cadenas de caracteres
	for(int i = 1;i < argc; i++){
        char *current_string = argv[i];
        char* end;
			long int value = strtol(argv[i],&end,10);
			if(*end == '\0'){
				// Valor transformado a entero
                push(list, current_string, value);
			}else{
                push(list, current_string, 0);
            }
        
		}
        
}

void printArray(struct LinkedList *list){
    print(list);
}

int main(int argc, char **argv)
{
	if(argc > 1){
        // Inicialización de la lista enlazada
        struct LinkedList * list;
        list = init();

		readArgs(argc, argv, list);
        printArray(list);

	}else{
		printf("No se pasaron argumentos\n");
        printf("El array: []\n");
	}
	
	return 0;
}
