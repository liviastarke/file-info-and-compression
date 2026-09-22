#ifndef __CHECK_POINTER_H__
#define __CHECK_POINTER_H__

#include <stdio.h>
#include <stdlib.h>

int check_pointer(void * ptr){
	if (!ptr){
		printf("Allocation error!\n");
		perror(NULL);
		exit(EXIT_FAILURE);
	}
	return 0;
}

#endif