#include <stdio.h>
#include <stdlib.h>

void readArgs(int argc, char **argv){
	for(int i = 1;i < argc; i++){
			char* end;
			//	strtol(
			//		cadena que queremos convertir, 
			//		puntero a cadena de caracteres de control que apuntará al primer carcater no numérico que encuentre,
			//		base numérica a la que queremos convertir la cadena
			//  ) -> Devuelve un long int
			long int value = strtol(argv[i],&end,10);
			if(!(*end != '\0')){
				printf("Argumento %d = %ld\n",i,value);
			}else{
				printf("Argumento %d = Error de conversión\n",i);
			}
			
		}
}

int main(int argc, char **argv)
{
	if(argc > 1){
		readArgs(argc, argv);
	}else{
		printf("No se pasaron argumentos\n");
	}
	
	return 0;
}
