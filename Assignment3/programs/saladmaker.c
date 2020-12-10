#include "saladmaker.h"


char* get_current_time(){

    time_t rawtime;
    struct tm * timeinfo;
    char* time_buffer = malloc(BUFFER_SIZE);
    memset(time_buffer, 0, BUFFER_SIZE);

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    
    sprintf(time_buffer, "%d:%d:%d:%d", timeinfo->tm_mday, timeinfo->tm_hour,
    timeinfo->tm_min, timeinfo->tm_sec);

    return time_buffer;

}




void saladmaker(int lb, int ub, int shmid, int no_sldmker){

    SharedSegment* shared_memory = SS_attach(shmid);
    if(shared_memory == NULL){
        assert(0);
        return;
    }

    shared_memory->saladmakers[no_sldmker-1].pid = getpid();

    char* filename = shared_memory->logfiles[no_sldmker-1];
    FILE* fp;

    char mess[BUFFER_SIZE];
    char* curr_time;
    int workbench_ingr;

    while(shared_memory->salads){

        fp = fopen(filename, "a+");

        printf("SALADMAKER %d: Waiting for ingredients...\n", no_sldmker);

        curr_time = get_current_time();
        sprintf(mess, "%s %d Saladmaker%d Waiting for ingredients\n", 
                curr_time, getpid(), no_sldmker);
        free(curr_time);
        fputs(mess,fp);


        P(&(shared_memory->saladmakers[no_sldmker-1].sem));
        workbench_ingr = shared_memory->workbench;

        if(shared_memory->salads <= 0){
            break;
        }

    
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

        curr_time = get_current_time();
        sprintf(mess, "%s %d Saladmaker%d Getting ingredients\n", 
                curr_time, getpid(), no_sldmker);
        free(curr_time);
        fputs(mess,fp);

        curr_time = get_current_time();
        sprintf(mess, "%s %d Saladmaker%d Start making salad\n", 
                curr_time, getpid(), no_sldmker);
        free(curr_time);
        fputs(mess,fp);

        printf("SALADMAKER %d: Preparing salad...\n", no_sldmker);
        sleep(ub - lb);
        printf("SALADMAKER %d: Salad is ready!\n", no_sldmker);

        curr_time = get_current_time();
        sprintf(mess, "%s %d Saladmaker%d End making salad\n", 
                curr_time, getpid(), no_sldmker);
        free(curr_time);
        fputs(mess,fp);

        shared_memory->saladmakers[no_sldmker-1].salads++;
        shared_memory->salads--;
        printf("Remaining salads: %d\n", shared_memory->salads);

        if(shared_memory->salads <= 0){
            V(&(shared_memory->chef));
            break;
        }
        

        V(&(shared_memory->chef));

        fclose(fp);
    }



    if(SS_detach(shared_memory) < 0){
        assert(0);
        return;
    }

}
