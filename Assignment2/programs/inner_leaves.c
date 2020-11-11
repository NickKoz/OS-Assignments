#include "common_types.h"

int counter_prime_algor = 1;

int main(int argc, char* argv[]){
    
    // int status;
    // int rc;
    // struct  pollfd  fdarray [1];

    int lb = atoi(argv[1]);
    int ub = atoi(argv[2]);
    int numOfChildren = atoi(argv[3]);
    int numI = atoi(argv[5]);

    char bufferlb[BUFFER_SIZE];
    char bufferub[BUFFER_SIZE];
    char bufferPrimeAlgor[BUFFER_SIZE];
    char* bufferPipe = argv[4];

    int d = (int)((ub-lb)/numOfChildren);
    if(d == 0){
        d = 1;
    }
    int templ = lb;
    int tempu = lb + d;

    char bufferPipeW[BUFFER_SIZE];
    // Creating children processes for each inner leaf.
    for(int i = 0 ; i < numOfChildren ; i++){

        sprintf(bufferPipeW, "bin/W%d", numI*numOfChildren + i);
        if(mkfifo(bufferPipeW, 0666) == -1){
            if(errno != EEXIST){
                printf("Fail mkfifo!\n");
                assert(0);
            }
        }

        pid_t W_id = fork();
        if(W_id < 0){
            printf("Error in fork!\n");
            exit(1);
        }
        // leaf's process
        if(W_id == 0){
            sprintf(bufferlb, "%d", templ);
            sprintf(bufferub, "%d", tempu);
            sprintf(bufferPrimeAlgor, "%d", counter_prime_algor);
            execl("bin/prime", "bin/prime", bufferlb, bufferub, bufferPrimeAlgor, bufferPipeW, (char*)NULL);
        }
        // inner leaf's process
        else{
            
            sprintf(bufferPipe, "bin/I%d", numI);
            int fd_I = open(bufferPipe, O_WRONLY);
            if(fd_I == -1){
                printf("Fail open of pipe\n");
                assert(0);
            }


            write(fd_I, &templ, sizeof(int));
            write(fd_I, &tempu, sizeof(int));


            sprintf(bufferPipeW, "bin/W%d", numI*numOfChildren + i);
            int fd_W = open(bufferPipeW, O_RDONLY);
            if(fd_W == -1){
                printf("Fail open of pipe\n");
                assert(0);
            }

            // while((wait(&status)) > 0);
            wait(NULL);
            
            if(templ > tempu || templ < 1){
                printf("HEREInner leaves!\n");
            }

            PMessage mess_received;
            for(int j = templ ; j <= tempu ; j++){
                read(fd_W, &mess_received, sizeof(PMessage));
                write(fd_I, &mess_received, sizeof(PMessage));
            }
            double w_total_time;

            read(fd_W, &w_total_time, sizeof(double));
            write(fd_I, &w_total_time, sizeof(double));

            close(fd_W);
            close(fd_I);

        }
        // We have 3 prime algorithms detectors.
        counter_prime_algor = counter_prime_algor == 3 ? 1  : counter_prime_algor + 1;
        
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
            if(tempu >= ub){
                tempu = ub;
            }
            else{
                tempu += d;
            }
        }
    }

}

