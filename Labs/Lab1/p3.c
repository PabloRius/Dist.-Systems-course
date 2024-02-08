#include <stdio.h>
#include <stdlib.h>

void readArgs(int argc, char **argv, int *array){
	for(int i = 1;i < argc; i++){
			char* end;
			long int value = strtol(argv[i],&end,10);
			if(*end == '\0'){
				// Valor transformado a entero
                array[i - 1] = value;
			}else{
				// El valor introducido no era un entero, ese slot se queda vacio
			}
			
		}
        
}

void printArray(int length, int *array){
    printf("[");
    for (int i = 0; i < length; i++){
            printf("%d",array[i]);
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
        int *array;

        // Le asignamos al array un espacio en memoria igual al tamaño de un entero * el número de argumentos
        array = (int *)malloc(array_length * sizeof(int));

		readArgs(argc, argv, array);
        printArray(array_length, array);

	}else{
		printf("No se pasaron argumentos\n");
        printf("El array: []\n");
	}
	
	return 0;
}
