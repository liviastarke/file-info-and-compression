#ifndef CHECK_POINTER_H
#define CHECK_POINTER_H

#include <stdio.h>
#include <stdlib.h>

static inline int check_pointer(void * ptr){
	if (!ptr){
		perror("Null pointer");
		exit(EXIT_FAILURE);
	}
	return 0;
}

#endif