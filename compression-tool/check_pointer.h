#ifndef CHECK_POINTER_H
#define CHECK_POINTER_H

#include <stdio.h>
#include <stdlib.h>

/**
 * For better debugging, might as well change to macro.
 * This allows printing the line and file where it failed.
 *

 #define CHECK_POINTER(ptr) \
	do{ \
		if (!(ptr)){ \
			fprintf(stderr, "Null pointer error in %s at line %d\n", __FILE__, __LINE__); \
			perror("NULL pointer"); \
			exit(EXIT_FAILURE); \
		} \
	} while (0)
*/

static inline void check_pointer(void * ptr){
	if (!ptr){
		perror("Null pointer");
		exit(EXIT_FAILURE);
	}
}

#endif