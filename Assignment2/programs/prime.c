#include "common_types.h"

#define YES 1
#define NO  0


int prime(int n, int prime_algor){
        int i;
        if (n==1)
            return(NO);
        int limit;
        if(prime_algor == 1)
            limit = n-1;
        else if(prime_algor == 2)
            limit = (int)sqrt((float)n);
        else
            limit = n/2;
        
        for(i=2 ; i<=limit ; i++)
            if ( n % i == 0) 
                return(NO);
        return(YES);
}

int main(int argc, char *argv[]){
    int lb=0, ub=0, i=0, prime_algor;

    if ( (argc != 5) ){
        printf("usage: prime1 lb ub\n");
        exit(1); }

    lb=atoi(argv[1]);
    ub=atoi(argv[2]);
    prime_algor = atoi(argv[3]);
    char* W_pipe = argv[4];

    if ( ( lb<1 )  || ( lb > ub ) ) {
        printf("usage: prime1 %d %d\n", lb, ub);
        // exit(1); 
    }
 
    int fd = open(W_pipe, O_WRONLY);
    if(fd == -1){
        printf("Fail open of pipe\n");
        assert(0);
    }
    clock_t begin, end;
    double time_spent, total_time = 0.0;
    int is_prime;
    for (i=lb ; i <= ub ; i++){

        begin = clock();
        is_prime = prime(i, prime_algor);
        end = clock();

        time_spent = (double)(end - begin)/ CLOCKS_PER_SEC;

        total_time += time_spent;

        PMessage mess;
        if (is_prime == YES){
            mess.prime_number = i;
            mess.time_taken = time_spent;
        }
        else{
            mess.prime_number = -1;
            mess.time_taken = 0.0;
        }   
        write(fd, &mess, sizeof(PMessage));
    }
    write(fd, &total_time, sizeof(double));

    close(fd);
}
