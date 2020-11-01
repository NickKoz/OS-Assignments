#include "common_types.h"


int main(int argc, char* argv[]){

    int lb, ub, numOfChildren;

    // Handling all arguments in any possible order.
    if(argc == 7){

        if(!strcmp(argv[1], "-l")){
            lb = atoi(argv[2]);
            if(!strcmp(argv[3], "-u")){
                ub = atoi(argv[4]);
                if(!strcmp(argv[5], "-w")){
                    numOfChildren = atoi(argv[6]);
                }
                else{
                    printf("Wrong execute of program!Try again\n");
                    return 1;
                }
            }
            else if(!strcmp(argv[3], "-w")){
                numOfChildren = atoi(argv[4]);
                if(!strcmp(argv[5], "-u")){
                    ub = atoi(argv[6]);
                }
                else{
                    printf("Wrong execute of program!Try again\n");
                    return 1;
                }
            }
            else{
                printf("Wrong execute of program!Try again\n");
                return 1;
            }
        }
        else if(!strcmp(argv[1], "-u")){
            ub = atoi(argv[2]);
            if(!strcmp(argv[3], "-l")){
                lb = atoi(argv[4]);
                if(!strcmp(argv[5], "-w")){
                    numOfChildren = atoi(argv[6]);
                }
                else{
                    printf("Wrong execute of program!Try again\n");
                    return 1;
                }
            }
            else if(!strcmp(argv[3], "-w")){
                numOfChildren = atoi(argv[4]);
                if(!strcmp(argv[5], "-l")){
                    lb = atoi(argv[6]);
                }
                else{
                    printf("Wrong execute of program!Try again\n");
                    return 1;
                }
            }
            else{
                printf("Wrong execute of program!Try again\n");
                return 1;
            }
        }
        else if(!strcmp(argv[1], "-w")){
            numOfChildren = atoi(argv[2]);
            if(!strcmp(argv[3], "-l")){
                lb = atoi(argv[4]);
                if(!strcmp(argv[5], "-u")){
                    ub = atoi(argv[6]);
                }
                else{
                    printf("Wrong execute of program!Try again\n");
                    return 1;
                }
            }
            else if(!strcmp(argv[3], "-u")){
                ub = atoi(argv[4]);
                if(!strcmp(argv[5], "-l")){
                    lb = atoi(argv[6]);
                }
                else{
                    printf("Wrong execute of program!Try again\n");
                    return 1;
                }
            }
            else{
                printf("Wrong execute of program!Try again\n");
                return 1;
            }
        }
        else{
            printf("Wrong execute of program!Try again\n");
            return 1;
        }
    }
    else{
        printf("Wrong execute of program!Try again\n");
        return 1;
    }


    if(numOfChildren == 0){
        printf("Number of children must be >= 1.\n");
        return 2;
    }


    find_primes(lb, ub, numOfChildren);

}
