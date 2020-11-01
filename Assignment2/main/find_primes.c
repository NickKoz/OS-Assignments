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


void make_pipe_for_process(){

    if(mkfifo("bin/C", 0777) == -1){
        if(errno != EEXIST){
            printf("Fail mkfifo!\n");
            assert(0);
        }
    }
}

void visit(Pointer p){
    PMessage* mess = p;
    printf("%d", mess->prime_number);
}

void destroy_mess(Pointer mess){
    free(mess);
}


void find_primes(int lb, int ub, int numOfChildren){

    LList primes = LL_create(destroy_mess);

    char bufferlb[BUFFER_SIZE];
    char bufferub[BUFFER_SIZE];
    char buffernum[BUFFER_SIZE];
    char bufferpipe[BUFFER_SIZE];
    sprintf(buffernum, "%d", numOfChildren);

    // Making C pipe for main process.
    make_pipe_for_process();


    int d = (int)(ub-lb)/numOfChildren;
    int templ = lb;
    int tempu = lb + d;

    for(int i = 0 ; i < numOfChildren ; i++){
        
        sprintf(bufferpipe, "bin/I%d", i);
        if(mkfifo(bufferpipe, 0777) == -1){
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
            execl("bin/inner_leaves", "bin/inner_leaves", bufferlb, bufferub, buffernum, bufferpipe, (char*)NULL);
        }
        // root process
        else{
            int fd = open("bin/C", O_RDONLY);
            if(fd == -1){
                printf("Fail open of pipe\n");
                assert(0);
            }

            int lprime,uprime;
            read(fd, &lprime, sizeof(int));
            read(fd, &uprime, sizeof(int));

            PMessage* messages = malloc((uprime - lprime)*sizeof(PMessage));
            for(int j = 0 ; j < uprime-lprime ; j++){
                read(fd, &messages[j], sizeof(PMessage));
                if(messages[j].prime_number != -1){
                    printf("%d READ FROM PIPE: %d %.6f\n", i ,messages[j].prime_number, messages[j].time_taken);
                    LL_insert_last(primes, &messages[j]);
                }
                wait(NULL);
            }
            free(messages);

            close(fd);
            wait(NULL);
        }
        
        templ = tempu + 1;
        if((i+1) == numOfChildren - 1 )
            tempu = ub;
        else
            tempu += d;
    }
    LL_visit(primes, visit);
}
