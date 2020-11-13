#include "common_types.h"


int main(int argc, char* argv[]){

    int lb, ub, num_of_children;

    // Handling all arguments in any possible order.
    if(argc == 7){

        if(!strcmp(argv[1], "-l")){
            lb = atoi(argv[2]);
            if(!strcmp(argv[3], "-u")){
                ub = atoi(argv[4]);
                if(!strcmp(argv[5], "-w")){
                    num_of_children = atoi(argv[6]);
                }
                else{
                    printf("Wrong execute of program!Try again\n");
                    return 1;
                }
            }
            else if(!strcmp(argv[3], "-w")){
                num_of_children = atoi(argv[4]);
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
                    num_of_children = atoi(argv[6]);
                }
                else{
                    printf("Wrong execute of program!Try again\n");
                    return 1;
                }
            }
            else if(!strcmp(argv[3], "-w")){
                num_of_children = atoi(argv[4]);
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
            num_of_children = atoi(argv[2]);
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


    if(num_of_children == 0){
        printf("Number of children must be >= 1.\n");
        return 2;
    }


    find_primes(lb, ub, num_of_children);

}
