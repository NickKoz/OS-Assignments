#include "List.h"

typedef struct Worker{

    char W[3];
    double time;

}Worker;

void print_worker(Worker w){
    printf("Time for %s: %f msecs\n", w.W, w.time);
}



int signals_recieved = 0;


void handler(int sig){

    if(sig == SIGUSR1){
        // printf("Recieved!\n");
        // fflush(stdout);
        signals_recieved++;
    }
}



void print_prime(PMessage p){
    printf("(%d,%.5f) ", p.prime_number, p.time_taken);
}



void find_primes(int lb, int ub, int num_of_children){

    char bufferlb[BUFFER_SIZE];
    char bufferub[BUFFER_SIZE];
    char buffernum[BUFFER_SIZE];
    char bufferpipe[BUFFER_SIZE];
    char bufferI[BUFFER_SIZE];
    char bufferRootProcessID[BUFFER_SIZE];
    sprintf(buffernum, "%d", num_of_children);
    // struct  pollfd  fdarray [1];
    // int rc;
    struct sigaction sa = {0};
    sa.sa_handler = handler;
    sa.sa_flags = SA_RESTART;
    sigaction(SIGUSR1, &sa, NULL);
    // signal(SIGUSR1, handler);

    int num_of_workers = num_of_children*num_of_children;
    int worker_it = 0;
    Worker* workers = malloc(num_of_workers*sizeof(Worker));


    int d = (int)((ub-lb)/num_of_children);
    if(d == 0){
        d = 1;
    }
    int templ = lb;
    int tempu = lb + d;

    printf("\nPrimes in [%d,%d] are: ", lb, ub);
    for(int i = 0 ; i < num_of_children ; i++){
        
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
            sprintf(bufferRootProcessID, "%d", getppid());
            execl("bin/inner_leaves", "bin/inner_leaves", bufferlb, bufferub, buffernum, bufferpipe, bufferI, bufferRootProcessID, (char*)NULL);
        }
        // root process
        else{

            sprintf(bufferpipe, "bin/I%d", i);
            int fd = open(bufferpipe, O_RDONLY);
            if(fd == -1){
                printf("Fail open of pipe\n");
                assert(0);
            }

            wait(NULL);


            PMessage mess;
            double total_time;
            for(int k = 0 ; k < num_of_children ; k++){
                while(true){
                    // Reading primes from pipe that I node sends.
                    read(fd, &mess, sizeof(PMessage));
                    if(mess.prime_number == END_PRIME){
                        break;
                    }
                    print_prime(mess);
                }
                read(fd, &total_time, sizeof(double));

                workers[worker_it].time = total_time;
                sprintf(bufferI, "W%d", i*num_of_children + k);
                strcpy(workers[worker_it++].W, bufferI);
            }
            close(fd);

        }
        
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

    // Finding min,max time of workers' processes.
    double min_time = 1000.0;
    double max_time = 0.0;
    for(int i = 0 ; i < num_of_workers ; i++){
        if(workers[i].time < min_time){
            min_time = workers[i].time;
        }
        if(workers[i].time > max_time){
            max_time = workers[i].time;
        }
    }
    printf("\n\nMin time for workers : %f msecs\n", min_time);
    printf("Max time for workers : %f msecs\n\n", max_time);

    int num_of_inner_nodes = num_of_children;
    int total_number_of_nodes = num_of_workers + num_of_inner_nodes;
    printf("Num of USR1 Recieved : %d/%d\n\n", signals_recieved, total_number_of_nodes);

    for(int i = 0 ; i < num_of_workers ; i++){
        print_worker(workers[i]);
    }

    free(workers);
}
