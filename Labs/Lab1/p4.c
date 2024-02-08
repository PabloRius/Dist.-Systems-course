#include <stdio.h>
#include <stdlib.h>

void readArgs(int argc, char **argv, char **array){
    // Ya que las cadenas de caracteres tienen un tamaño indeterminado, 
    // el array contendrá punteros a estas cadenas de caracteres
	for(int i = 1;i < argc; i++){
        char *current_string = argv[i];
        array[i - 1] = current_string;
		}
        
}

void printArray(int length, char **array){
    printf("[");
    for (int i = 0; i < length; i++){
            printf("%s",array[i]);
            if(i < length - 1){
                printf(", ");
            }
        }
    printf("]\n");
}

int main(int argc, char **argv)
{
	if(argc > 1){
        // Definimos el tamaño del array previamente contando el número de parámetros que introduce el usuario
        int array_length = argc - 1;
        // Es un puntero (array) de punteros
        char **array;

        // Le asignamos al array un espacio en memoria igual al tamaño de un entero * el número de argumentos
        array = (char **)malloc(array_length * sizeof(char *));

		readArgs(argc, argv, array);
        printArray(array_length, array);

	}else{
		printf("No se pasaron argumentos\n");
        printf("El array: []\n");
	}
	
	return 0;
}
