#include "chef.h"
#include "common.h"


int temp_workbench = -1;

int select_ingredients(){

    int workbench;
    do{
        workbench = rand() % (ONIONS_PEPPERS + 1) + TOMATOES_PEPPERS;
    }while(workbench == temp_workbench);

    temp_workbench = workbench;
    
    return workbench;
}


void takes_breath(int mantime){
    sleep(mantime);
}



void chef(int numofSlds, double mantime){

    int shmid = SS_create();
    if(shmid == -1){
        assert(0);
        return;
    }
    srand(time(NULL));

    printf("CHEF: Shared Memory ID is %d\n", shmid);

    SharedSegment* shared_memory = SS_attach(shmid);
    if(shared_memory == NULL){
        assert(0);
        return;
    }

    SS_initialize(shared_memory, numofSlds);

    int work;
    int notified_slmker;
    char mess[BUFFER_SIZE];

    while(shared_memory->salads){

        work = select_ingredients();
        shared_memory->workbench = work;

        
        if(work == TOMATOES_PEPPERS){
            shared_memory->shared_ingreds.tomatoes++;
            shared_memory->shared_ingreds.peppers++;
            printf("CHEF: 1 tomato and 1 pepper are available on workbench.\n");
            sprintf(mess, "Chef Selecting ingredients ntomata piperia");
            write_to_log(mess, GLOBAL_LOG, shared_memory);
            V(&(shared_memory->saladmakers[0].sem));
            notified_slmker = 1;
        }
        else if(work == TOMATOES_ONIONS){
            shared_memory->shared_ingreds.tomatoes++;
            shared_memory->shared_ingreds.onions++;
            printf("CHEF: 1 tomato and 1 onion are available on workbench.\n");
            sprintf(mess, "Chef Selecting ingredients ntomata kremudi");
            write_to_log(mess, GLOBAL_LOG, shared_memory);
            V(&(shared_memory->saladmakers[1].sem));
            notified_slmker = 2;
        }
        else if(work == ONIONS_PEPPERS){
            shared_memory->shared_ingreds.onions++;
            shared_memory->shared_ingreds.peppers++;
            printf("CHEF: 1 onion and 1 pepper are available on workbench.\n");
            sprintf(mess, "Chef Selecting ingredients kremudi piperia");
            write_to_log(mess, GLOBAL_LOG, shared_memory);
            V(&(shared_memory->saladmakers[2].sem));
            notified_slmker = 3;
        }

        sprintf(mess, "Chef Notify saladmaker #%d", notified_slmker);
        write_to_log(mess, GLOBAL_LOG, shared_memory);
        
        sprintf(mess, "Chef Man time for resting");
        write_to_log(mess, GLOBAL_LOG, shared_memory);

        printf("CHEF: Resting...\n");
        takes_breath(mantime);
        
        if(shared_memory->salads <= 0){
            break;
        }

        P(&(shared_memory->chef));
    }

    int value = 0;
    for(int i = 0 ; i < NUM_OF_SALADMAKERS ; i++){
        sem_getvalue(&(shared_memory->saladmakers[i].sem), &value);
        if(value <= 0){
            V(&(shared_memory->saladmakers[i].sem));
        }
    }




    if(shared_memory->salads < 0){
        shared_memory->salads *= -1;
    }

    printf("\nTotal #salads: %d\n", shared_memory->salads + numofSlds);
    pid_t pid;
    int salads;
    for(int i = 0 ; i < NUM_OF_SALADMAKERS ; i++){
        pid = shared_memory->saladmakers[i].pid;
        salads = shared_memory->saladmakers[i].salads;
        printf("#salads of salad_maker%d %d : %d\n", i+1, pid, salads);
    }


    if(SS_detach(shared_memory) < 0){
        assert(0);
        return;
    }

    if(SS_destroy(shared_memory, shmid) < 0){
        assert(0);
        return;
    }

}
