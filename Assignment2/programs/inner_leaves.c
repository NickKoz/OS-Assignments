#include "common_types.h"

int counter_prime_algor = 1;

int main(int argc, char* argv[]){

    int lb = atoi(argv[1]);
    int ub = atoi(argv[2]);
    int numOfChildren = atoi(argv[3]);

    char bufferlb[BUFFER_SIZE];
    char bufferub[BUFFER_SIZE];
    char bufferPrimeAlgor[BUFFER_SIZE];
    char* bufferPipe = argv[4];

    int d = (int)(ub-lb)/numOfChildren;

    int templ = lb;
    int tempu = lb + d;

    // printf("LowBound = %d\nUpperBound = %d\nnumOfChildren = %d\n", lb, ub , numOfChildren);

    // Creating children processes for each inner leaf.
    for(int i = 0 ; i < numOfChildren ; i++){

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
            execl("bin/prime", "bin/prime", bufferlb, bufferub, bufferPrimeAlgor, bufferPipe, (char*)NULL);
        }
        // inner leaf's process
        else{
            int fd_C = open("bin/C", O_WRONLY);
            if(fd_C == -1){
                printf("Fail open of pipe\n");
                assert(0);
            }

            write(fd_C, &templ, sizeof(int));
            write(fd_C, &tempu, sizeof(int));

            int fd = open(bufferPipe, O_RDONLY);
            if(fd == -1){
                printf("Fail open of pipe\n");
                assert(0);
            }
            PMessage mess_received;
            for(int j = templ ; j <= tempu ; j++){
                read(fd, &mess_received, sizeof(PMessage));
                if(mess_received.prime_number != -1){
                    printf("%d-%d  %d READ FROM PIPE: %d %.6f\n",templ, tempu,  i ,mess_received.prime_number, mess_received.time_taken);
                }
                write(fd_C, &mess_received, sizeof(PMessage));
            }
            double w_total_time;
            read(fd, &w_total_time, sizeof(double));
            printf("Total Time of W%d : %.6f\n", i, w_total_time);
            close(fd);
            // while((W_id = wait(&status)) > 0);

            wait(NULL);
        }
        // We have 3 prime algorithms detectors.
        counter_prime_algor = counter_prime_algor == 3 ? 1 : counter_prime_algor + 1;
        templ = tempu + 1;
        if((i+1) == numOfChildren - 1 )
            tempu = ub;
        else
            tempu += d;
    }
} 

