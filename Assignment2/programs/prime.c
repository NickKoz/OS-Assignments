#include "common_types.h"

#define YES 1
#define NO  0


int prime(int n, int prime_algor){
        int i;
        if (n==1)
            return(NO);
        int limit;
        if(prime_algor == 1)
            limit = n-1;    // checking all numbers till n.
        else if(prime_algor == 2)
            limit = (int)sqrt((float)n);    // checking numbers till sqrt(n).
        else
            limit = n/2;    // checking half numbers.
        
        for(i=2 ; i<=limit ; i++)
            if ( n % i == 0) 
                return(NO);
        return(YES);
}

int main(int argc, char *argv[]){

    int lb = atoi(argv[1]);
    int ub = atoi(argv[2]);
    int prime_algor = atoi(argv[3]);
    char* W_pipe = argv[4];
    int root_process_id = atoi(argv[5]);

 
    int fd = open(W_pipe, O_WRONLY);
    if(fd == -1){
        printf("Fail open of pipe\n");
        assert(0);
    }
    clock_t begin, end;
    double time_spent, total_time = 0.0;
    int is_prime;
    for (int i = lb ; i <= ub ; i++){
        // Calculating time for finding if number is prime or not.
        begin = clock();
        is_prime = prime(i, prime_algor);
        end = clock();

        // We need time to be to msecs. So we multiply secs by 1000.
        time_spent = (double)(end - begin)/ CLOCKS_PER_SEC * 1000.0;

        total_time += time_spent;   // calculate total time of process

        PMessage mess;
        if (is_prime == YES){
            mess.prime_number = i;
            mess.time_taken = time_spent;
        }
        else{
            // If number is not prime, we return no prime value.
            mess.prime_number = NO_PRIME;
            mess.time_taken = 0.0;
        }   
        write(fd, &mess, sizeof(PMessage)); // we write number to pipe.
    }
    write(fd, &total_time, sizeof(double)); // we write total time to pipe too

    close(fd);
    // Sending signal to root process before W process terminates.
    kill(root_process_id, SIGUSR1);
}
