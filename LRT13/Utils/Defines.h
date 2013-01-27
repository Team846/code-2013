#ifndef DEFINES_H_
#define DEFINES_H_

#define DEBUG

#define DELETE( ptr ) \
	delete ptr; \
	ptr = NULL;

#define DELETE_ARR( ptr ) \
	delete[] ptr; \
	ptr = NULL;

#endif
