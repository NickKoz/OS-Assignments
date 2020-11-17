#ifndef COMMON_TYPES
#define COMMON_TYPES

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>

#define BUFFER_SIZE 256

#define NO_PRIME -1

#define END_PRIME -1


typedef struct PMessage{

    int prime_number;
    double time_taken;

}PMessage;


typedef void* Pointer;

typedef void (*DestroyFunc)(Pointer);
typedef int (*CompareFunc)(Pointer,Pointer);
typedef void (*VisitFunc)(Pointer);


void find_primes(int, int, int);


#endif