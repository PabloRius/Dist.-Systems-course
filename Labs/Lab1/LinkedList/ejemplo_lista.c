#include <stdio.h> 
#include <stdlib.h>
#include "list.h"

int main(void) 
{

	List my_list;
	int err = 0;
	int i;
	char key[MAX_KEY_LENGTH];



	err = init(&my_list);
	if (err == -1) {
		printf("Error in init\n");
		return -1;
	}

	for (i=0; i < 10; i++){
		sprintf(key, "%d", i);
		err = set(&my_list, key, i);
		if (err == -1) {
			printf("Error in set\n");
			return -1;
		}
	}

	printList(my_list);

	for (i=0; i < 10; i++){
		sprintf(key, "%d", i);
		err = delete(&my_list, key);
		if (err == -1) {
			printf("Error in delete\n");
			return -1;
		}
	}


	err = destroy(&my_list);
	if (err == -1) {
		printf("Error in destroy\n");
		return -1;
	}
	return 0;
}



