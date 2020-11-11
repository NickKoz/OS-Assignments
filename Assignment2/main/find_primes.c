#include "List.h"

char* concat(const char* str1, const char* str2){

    char* result = malloc(strlen(str1) + strlen(str2) + 1);
    if(result == NULL){
        return NULL;
    }

    strcpy(result, str1);
    strcat(result, str2);

    return result;
}



void visit(Pointer p){
    PMessage* mess = p;
    printf("(%d,%f)", mess->prime_number, mess->time_taken);
}

void destroy_mess(Pointer mess){
    free(mess);
}


void find_primes(int lb, int ub, int numOfChildren){
    // int status;
    LList primes = LL_create(destroy_mess);

    char bufferlb[BUFFER_SIZE];
    char bufferub[BUFFER_SIZE];
    char buffernum[BUFFER_SIZE];
    char bufferpipe[BUFFER_SIZE];
    char bufferI[BUFFER_SIZE];
    sprintf(buffernum, "%d", numOfChildren);
    // struct  pollfd  fdarray [1];
    // int rc;


    int d = (int)((ub-lb)/numOfChildren);
    if(d == 0){
        d = 1;
    }
    int templ = lb;
    int tempu = lb + d;

    for(int i = 0 ; i < numOfChildren ; i++){
        
        sprintf(bufferpipe, "bin/I%d", i);
        if(mkfifo(bufferpipe, 0666) == -1){
            if(errno != EEXIST){
                printf("Fail mkfifo!\n");
                assert(0);
            }
        }

        pid_t I_id = fork();
        if(I_id < 0){
            printf("Error in fork!\n");
            exit(1);
        }
        // inner leaf's process
        if(I_id == 0){
            sprintf(bufferlb, "%d", templ);
            sprintf(bufferub, "%d", tempu);
            sprintf(bufferI, "%d", i);
            execl("bin/inner_leaves", "bin/inner_leaves", bufferlb, bufferub, buffernum, bufferpipe, bufferI, (char*)NULL);
        }
        // root process
        else{

            sprintf(bufferpipe, "bin/I%d", i);
            int fd = open(bufferpipe, O_RDONLY);
            if(fd == -1){
                printf("Fail open of pipe\n");
                assert(0);
            }
            // while((wait(&status)) > 0);
            wait(NULL);

            int lprime,uprime;

            PMessage mess;
            double total_time;
            for(int k = 0 ; k < numOfChildren ; k++){
                read(fd, &lprime, sizeof(int));
                read(fd, &uprime, sizeof(int));
                for(int j = lprime ; j <= uprime ; j++){
                    read(fd, &mess, sizeof(PMessage));
                    
                    if(mess.prime_number != -1){
                        PMessage* temp_mess = malloc(sizeof(PMessage));
                        temp_mess->prime_number = mess.prime_number;
                        temp_mess->time_taken = mess.time_taken;
                        LL_insert_last(primes, temp_mess);
                    }
                }
                read(fd, &total_time, sizeof(double));
                printf("Total time of W%d : %.6f\n", i*numOfChildren + k, total_time);
            }
            close(fd);

        }
        
        if((templ <= lb) && (i != 0)){
            templ = lb;
        }
        else{
            templ = tempu + 1;
        }
        
        if((i+1) == numOfChildren - 1 ){
            tempu = ub;
        }
        else{
            tempu += d;
        }
    }


    LL_visit(primes, visit);
    LL_destroy(primes);
}
    
            

