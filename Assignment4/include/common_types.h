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
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <errno.h>
#include <limits.h>

#define BUFFER_SIZE 512

#include "common.h"


void quic(char*, char*);
bool check_if_dir(char*);


typedef void* Pointer;

typedef void (*DestroyFunc)(Pointer);
typedef int (*CompareFunc)(Pointer,Pointer);
typedef void (*VisitFunc)(Pointer);





#endif
