#include "List.h"


void destroy_mess(Pointer mess){
    free(mess);
}


int counter_prime_algor = 1;

int main(int argc, char* argv[]){

    int lb = atoi(argv[1]);
    int ub = atoi(argv[2]);
    int num_of_children = atoi(argv[3]);
    int numI = atoi(argv[5]);

    char bufferlb[BUFFER_SIZE];
    char bufferub[BUFFER_SIZE];
    char bufferPrimeAlgor[BUFFER_SIZE];
    char bufferRootProcessID[BUFFER_SIZE];
    int root_process_id = atoi(argv[6]);
    char* bufferPipe = argv[4];

    int d = (int)((ub-lb)/num_of_children);
    if(d == 0){
        d = 1;
    }
    int templ = lb;
    int tempu = lb + d;

    char bufferPipeW[BUFFER_SIZE];
    // Creating children processes for each inner leaf.
    for(int i = 0 ; i < num_of_children ; i++){

        sprintf(bufferPipeW, "bin/W%d", numI*num_of_children + i);
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
            sprintf(bufferRootProcessID, "%d", root_process_id);
            execl("bin/prime", "bin/prime", bufferlb, bufferub, bufferPrimeAlgor, bufferPipeW, bufferRootProcessID, (char*)NULL);
        }
        // inner leaf's process
        else{
            
            sprintf(bufferPipe, "bin/I%d", numI);
            int fd_I = open(bufferPipe, O_WRONLY);
            if(fd_I == -1){
                printf("Fail open of pipe\n");
                assert(0);
            }


            sprintf(bufferPipeW, "bin/W%d", numI*num_of_children + i);
            int fd_W = open(bufferPipeW, O_RDONLY);
            if(fd_W == -1){
                printf("Fail open of pipe\n");
                assert(0);
            }

            wait(NULL); // waits for leaf process to finish.

            // Reading and adding each prime that W process found into a list.
            LList list_of_primes = LL_create(destroy_mess);
            
            PMessage mess_received;
            for(int j = templ ; j <= tempu ; j++){
                // Reading prime numbers from worker process.
                read(fd_W, &mess_received, sizeof(PMessage));
                // We keep primes in a list.
                if(mess_received.prime_number != NO_PRIME){
                    PMessage* temp_mess = malloc(sizeof(PMessage));
                    temp_mess->prime_number = mess_received.prime_number;
                    temp_mess->time_taken = mess_received.time_taken;
                    LL_insert_last(list_of_primes, temp_mess);
                }
            }

            // We sent primes to root by pipe.
            PMessage mess_to_be_sent;
            PMessage* temp_data;
            for(LLNode temp = LL_first(list_of_primes) ; temp != NULL ; temp = LLNode_next(temp)){
                temp_data = LLNode_get_data(temp);
                mess_to_be_sent.prime_number = temp_data->prime_number; 
                mess_to_be_sent.time_taken = temp_data->time_taken; 
                write(fd_I, &mess_to_be_sent, sizeof(PMessage));
            }
            // Sending end value to terminate sending prime numbers to root process. 
            mess_to_be_sent.prime_number = END_PRIME;
            write(fd_I, &mess_to_be_sent, sizeof(PMessage));


            double w_total_time;

            read(fd_W, &w_total_time, sizeof(double));
            // Send W' process time to root.
            write(fd_I, &w_total_time, sizeof(double));

            close(fd_W);
            close(fd_I);

            LL_destroy(list_of_primes);
        }
        // We have 3 prime algorithms.
        counter_prime_algor = counter_prime_algor == 3 ? 1  : counter_prime_algor + 1;
        
        if((templ <= lb) && (i != 0)){
            templ = lb;
        }
        else{
            templ = tempu + 1;
        }
        
        if((i+1) == num_of_children - 1 ){
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
    // Sending signal to root process before I process terminates.
    kill(root_process_id, SIGUSR1);
}

