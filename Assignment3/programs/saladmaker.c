#include "saladmaker.h"
#include "common.h"



static void prepare_salad(int lb, int ub){
    sleep(ub - lb);
}




void saladmaker(int lb, int ub, int shmid, int no_sldmker){

    SharedSegment* shared_memory = SS_attach(shmid);
    if(shared_memory == NULL){
        assert(0);
        return;
    }

    shared_memory->saladmakers[no_sldmker-1].pid = getpid();


    char mess[BUFFER_SIZE];
    int workbench_ingr;

    while(shared_memory->salads){

        printf("SALADMAKER %d: Waiting for ingredients...\n", no_sldmker);

        sprintf(mess, "Saladmaker%d Waiting ingredients", no_sldmker);
        write_to_log(mess, no_sldmker, shared_memory);

        P(&(shared_memory->saladmakers[no_sldmker-1].sem));
        workbench_ingr = shared_memory->workbench;

        if(shared_memory->salads <= 0){
            break;
        }
        
       
        sprintf(mess, "Saladmaker%d Getting ingredients", no_sldmker);
        write_to_log(mess, no_sldmker, shared_memory);
        write_to_log(mess, GLOBAL_LOG, shared_memory);

    
        if(workbench_ingr == TOMATOES_PEPPERS && no_sldmker == 1){
            shared_memory->shared_ingreds.tomatoes--;
            shared_memory->shared_ingreds.peppers--;
            printf("SALADMAKER %d: Recieved 1 tomato and 1 pepper.\n", no_sldmker);
        }
        else if(workbench_ingr == TOMATOES_ONIONS && no_sldmker == 2){
            shared_memory->shared_ingreds.tomatoes--;
            shared_memory->shared_ingreds.onions--;
            printf("SALADMAKER %d: Recieved 1 tomato and 1 onion.\n", no_sldmker);
        }
        else if(workbench_ingr == ONIONS_PEPPERS && no_sldmker == 3){
            shared_memory->shared_ingreds.onions--;
            shared_memory->shared_ingreds.peppers--;
            printf("SALADMAKER %d: Recieved 1 onion and 1 pepper.\n", no_sldmker);
        }

        V(&(shared_memory->chef));  // Notifies chef that ingredients are recieved.

        sprintf(mess, "Saladmaker%d Start making salad", no_sldmker);
        write_to_log(mess, no_sldmker, shared_memory);
        write_to_log(mess, GLOBAL_LOG, shared_memory);

        printf("SALADMAKER %d: Preparing salad...\n", no_sldmker);
        prepare_salad(lb, ub);
        printf("SALADMAKER %d: Salad is ready!\n", no_sldmker);

        sprintf(mess, "Saladmaker%d End making salad",  no_sldmker);
        write_to_log(mess, no_sldmker, shared_memory);
        write_to_log(mess, GLOBAL_LOG, shared_memory);


        shared_memory->saladmakers[no_sldmker-1].salads++;
        shared_memory->salads--;
        printf("Remaining salads: %d\n", shared_memory->salads);

        if(shared_memory->salads <= 0){
            V(&(shared_memory->chef));
            break;
        }
        


    }



    if(SS_detach(shared_memory) < 0){
        assert(0);
        return;
    }

}
