#ifndef COMMON_TYPES
#define COMMON_TYPES

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <limits.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <signal.h>

#define BUFFER_SIZE 256

#include "shared_memory.h"


typedef void* Pointer;

typedef void (*DestroyFunc)(Pointer);
typedef int (*CompareFunc)(Pointer,Pointer);
typedef void (*VisitFunc)(Pointer);





#endif
