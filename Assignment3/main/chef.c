#include "chef.h"
#include "common.h"
#include "List.h"


int temp_workbench = -1;

static int select_ingredients(){

    int workbench;
    // Workbench should not have same ingredients twice in a row.
    do{
        workbench = rand() % (ONIONS_PEPPERS + 1) + TOMATOES_PEPPERS;
    }while(workbench == temp_workbench);

    temp_workbench = workbench;
    
    return workbench;
}


static void takes_breath(int mantime){
    sleep(mantime);
}


// Returns lines' number of a file.
int file_lines(FILE* fp){   

    int line_counter = 0;
    char ch;

    while(feof(fp) == 0){
        // We read each character.
        ch = fgetc(fp);
        // If we reach \n, we have a line.
        if(ch == '\n')
            line_counter++;
    }
    // Resets file pointer to the start of the file.
    rewind(fp);
    return line_counter;
}


typedef struct{

    char lb[BUFFER_SIZE];
    char ub[BUFFER_SIZE];
    int procs;  // number of concurrent processes
    
}TimePeriod;


void destroy_period(LLItem pointer){
    free(pointer);
}

void visit_tperiod(LLItem pointer){

    TimePeriod* tperiod = pointer;

    printf("[%s, %s] : %d concurrent saladmakers", 
    tperiod->lb, tperiod->ub, tperiod->procs);

}


// Returns list with time periods in which 2 or more processes run concurrently.
static LList get_concurrent_processes(char* global_log){

    FILE* fp = fopen(global_log, "r");
    if(fp == NULL){
        return NULL;
    }

    int number_of_events = file_lines(fp);
    
    // A buffer used for fetching each line of file.
    char buffer_event[BUFFER_SIZE] = {0};

    LList time_periods = LL_create(destroy_period);

    const char delim[2] = " ";
    char *time_format, *process_name, *message;
    int no_sldmker;

    int concurrent_proc = 0;
    int prev_sldmaker = -1;
    char temp_lb[BUFFER_SIZE] = {0};
    char temp_ub[BUFFER_SIZE] = {0};
    TimePeriod* tperiod;

    for(int i = 0 ; i < number_of_events ; i++){

        // We fetch each event line of file.
        fgets(buffer_event, BUFFER_SIZE, fp);

        time_format = strtok(buffer_event, delim);
        process_name = strtok(NULL, delim);
        process_name = strtok(NULL, delim);
        message = strtok(NULL, delim);

        // We don't need chef in concurrent processes.
        if(!strcmp(process_name, "Chef")){
            continue;
        }
        
        // Extracting saladmaker's number.
        no_sldmker = process_name[10] - '0';

        // Extracting info about concurrent processes.

        if(!strcmp(message, "Getting")){
            concurrent_proc++;
            if(prev_sldmaker == -1){
                prev_sldmaker = no_sldmker;
                strcpy(temp_lb, time_format);
            }
            continue;
        }
        else if(!strcmp(message, "End")){
            if(prev_sldmaker == no_sldmker && concurrent_proc != 1){
                tperiod = malloc(sizeof(TimePeriod));
                strcpy(tperiod->lb, temp_lb);
                strcpy(temp_ub, time_format);
                strcpy(tperiod->ub, temp_ub);
                tperiod->procs = concurrent_proc;

                LL_insert_last(time_periods, tperiod);

                concurrent_proc = 0;
                prev_sldmaker = -1;
            }
            else if(prev_sldmaker == no_sldmker && concurrent_proc == 1){
                concurrent_proc = 0;
                prev_sldmaker = -1;
            }
            else if(prev_sldmaker != no_sldmker){
                concurrent_proc++;
            }
        }

    }

    fclose(fp);

    return time_periods;
}



void chef(int numofSlds, double mantime){

    int shmid = SS_create();
    if(shmid == -1){
        assert(0);
        return;
    }
    
    srand(time(NULL));

    printf("CHEF: Shared Memory ID is %d\n", shmid);

    SharedSegment* shared_resources = SS_attach(shmid);
    if(shared_resources == NULL){
        assert(0);
        return;
    }

    SS_initialize(shared_resources, numofSlds);

    int work;
    int notified_slmker;
    char mess[BUFFER_SIZE];

    while(shared_resources->salads){

        work = select_ingredients();
        shared_resources->workbench_ingredients = work;

        // Notifies appropriate saladmaker.

        if(work == TOMATOES_PEPPERS){
            printf("CHEF: 1 tomato and 1 pepper are available on workbench.\n");
            sprintf(mess, "Chef Selecting ingredients ntomata piperia");
            write_to_log(mess, GLOBAL_LOG, shared_resources);
            V(&(shared_resources->saladmakers[0].sem));
            notified_slmker = 1;
        }
        else if(work == TOMATOES_ONIONS){
            printf("CHEF: 1 tomato and 1 onion are available on workbench.\n");
            sprintf(mess, "Chef Selecting ingredients ntomata kremudi");
            write_to_log(mess, GLOBAL_LOG, shared_resources);
            V(&(shared_resources->saladmakers[1].sem));
            notified_slmker = 2;
        }
        else if(work == ONIONS_PEPPERS){
            printf("CHEF: 1 onion and 1 pepper are available on workbench.\n");
            sprintf(mess, "Chef Selecting ingredients kremudi piperia");
            write_to_log(mess, GLOBAL_LOG, shared_resources);
            V(&(shared_resources->saladmakers[2].sem));
            notified_slmker = 3;
        }

        sprintf(mess, "Chef Notify saladmaker #%d", notified_slmker);
        write_to_log(mess, GLOBAL_LOG, shared_resources);

        P(&(shared_resources->chef));  // Waits for saladmaker to recieve ingredients

        sprintf(mess, "Chef Man time for resting");
        write_to_log(mess, GLOBAL_LOG, shared_resources);

        printf("CHEF: Resting...\n");
        takes_breath(mantime);
        
        if(shared_resources->salads <= 0){
            printf("CHEF: We have enough salads.\n");
            break;
        }

    }

    // Wake up any saladmaker that are waiting for ingredients.
    int value = 0;
    for(int i = 0 ; i < NUM_OF_SALADMAKERS ; i++){
        sem_getvalue(&(shared_resources->saladmakers[i].sem), &value);
        if(value <= 0){
            V(&(shared_resources->saladmakers[i].sem));
        }
    }


    // Prints results.

    if(shared_resources->salads < 0){
        shared_resources->salads *= -1;
    }

    printf("\nTotal #salads: %d\n\n", shared_resources->salads + numofSlds);
    pid_t pid;
    int salads;
    for(int i = 0 ; i < NUM_OF_SALADMAKERS ; i++){
        pid = shared_resources->saladmakers[i].pid;
        salads = shared_resources->saladmakers[i].salads;
        printf("#salads of salad_maker%d %d : %d\n", i+1, pid, salads);
    }

    printf("\nTime periods with concurrent processes are: \n");
    
    LList conc_processes = get_concurrent_processes(shared_resources->global_logfile);

    LL_visit(conc_processes, visit_tperiod);

    LL_destroy(conc_processes);


    // Detach shared memory.
    if(SS_detach(shared_resources) < 0){
        assert(0);
        return;
    }

    // Free shared memory.
    if(SS_destroy(shared_resources, shmid) < 0){
        assert(0);
        return;
    }

}
