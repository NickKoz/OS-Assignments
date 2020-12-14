#include "shared_memory.h"

// Allocates shared segment. Returns shared memory ID or -1 in case of fail.
int SS_create(){

    int shmid = shmget(IPC_PRIVATE, sizeof(SharedSegment), IPC_CREAT | 0660);
    if(shmid == -1){
        printf("Shared memory creation failed!\n");
    }

    return shmid;
}

SharedSegment* SS_attach(int shmid){

    SharedSegment* shared_memory = shmat(shmid, (void*)0, 0);
    if(shared_memory == NULL){
        printf("Shared memory attachment failed!\n");
    }
    return shared_memory;
}


void SS_initialize(SharedSegment* shared_memory, int numofSlds){    

    shared_memory->salads = numofSlds;

    shared_memory->workbench_ingredients = EMPTY;

    char temp_log[BUFFER_SIZE] = {0};
    FILE* temp_fp;
    for(int i = 0 ; i < NUM_OF_SALADMAKERS ; i++){
        sprintf(temp_log, "log%d.txt", i+1);
        strcpy(shared_memory->logfiles[i], temp_log);
        temp_fp = fopen(temp_log, "w");   // create new file or overwrite
        if(temp_fp == NULL){
            printf("Error with log file!\n");
            assert(0);
        }
        fclose(temp_fp);
    }

    // Creating global log file.
    memset(temp_log, 0, BUFFER_SIZE);
    sprintf(temp_log, "global_log.txt");
    strcpy(shared_memory->global_logfile, temp_log);
    temp_fp = fopen(temp_log, "w"); // create new file or overwrite
    if(temp_fp == NULL){
        printf("Error with log file!\n");
        assert(0);
    }
    fclose(temp_fp);
    
    
    for(int i = 0 ; i < NUM_OF_SALADMAKERS ; i++){
        sem_init(&(shared_memory->saladmakers[i].sem), 1, 0);
        shared_memory->saladmakers[i].salads = 0;
    }
    
    sem_init(&(shared_memory->chef), 1, 0);

}


int SS_detach(SharedSegment* shared_memory){
    if(shmdt(shared_memory) < 0){
        return -1;
    }
    return 1;
}



int SS_destroy(SharedSegment* shared_memory, int shmid){

    int err = shmctl(shmid, IPC_RMID, 0);
    if(err == -1){
        printf("Cannot remove segment\n");
        return -1;
    }

    for(int i = 0 ; i < NUM_OF_SALADMAKERS ; i++)
        sem_destroy(&(shared_memory->saladmakers[i].sem));
    
    sem_destroy(&(shared_memory->chef));

    return 1;
}




void P(sem_t* sem){
    sem_wait(sem);
}


void V(sem_t* sem){
    sem_post(sem);
}
