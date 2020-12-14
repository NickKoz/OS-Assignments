#ifndef SHARED_MEMORY
#define SHARED_MEMORY

#include "common_types.h"

#define NUM_OF_SALADMAKERS 3
#define GLOBAL_LOG -1
#define EMPTY -1


enum ingredients_comb{TOMATOES_PEPPERS, TOMATOES_ONIONS, ONIONS_PEPPERS};



typedef struct{

    sem_t sem;
    int salads;
    pid_t pid;

}Saladmaker;



typedef struct{

    int workbench_ingredients;

    int salads; // Salads that have to be made.

    char logfiles[NUM_OF_SALADMAKERS][BUFFER_SIZE];  // 1 log file for each saladmaker
    char global_logfile[BUFFER_SIZE];

    Saladmaker saladmakers[NUM_OF_SALADMAKERS];

    sem_t chef;

}SharedSegment; // or SS


int SS_create();
SharedSegment* SS_attach(int);
void SS_initialize(SharedSegment*, int);
int SS_detach(SharedSegment*);
int SS_destroy(SharedSegment*,int);


void P(sem_t*);
void V(sem_t*);


#endif
