#ifndef COMMON_TYPES
#define COMMON_TYPES

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <time.h>

typedef void* Pointer;

typedef void (*DestroyFunc)(Pointer);
typedef int (*CompareFunc)(Pointer,Pointer);
typedef void (*VisitFunc)(Pointer);


void add_users_files_data(char*, char*);
void prompt();


#endif