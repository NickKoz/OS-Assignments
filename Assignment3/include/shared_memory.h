#ifndef SHARED_MEMORY
#define SHARED_MEMORY

#include "common_types.h"

enum ingredients_comb{TOMATOES_PEPPERS, TOMATOES_ONIONS, ONIONS_PEPPERS};


typedef struct{

    // Ingredients on workbench.
    int tomatoes;
    int peppers;
    int onions;

}SharedIngredients;


typedef struct{

    sem_t sem;
    int salads;
    pid_t pid;

}Saladmaker;



typedef struct{

    SharedIngredients shared_ingreds;

    int workbench;

    int salads; // Salads that have to be made.

    sem_t sem_salads;

    char logfiles[NUM_OF_SALADMAKERS][BUFFER_SIZE];  // 1 log file for each saladmaker

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
