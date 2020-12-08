#include "saladmaker.h"


int main(int argc, char* argv[]){

    int lb,ub,shmid,no_sldmker;

    // Handling all arguments in any possible order.
    if(argc == 9){
        for(int i = 1 ; i < argc ; i += 2){
            if(!strcmp(argv[i], "-t1")){
                lb = atoi(argv[i+1]);
            }
            else if(!strcmp(argv[i], "-t2")){
                ub = atoi(argv[i+1]);
            }
            else if(!strcmp(argv[i], "-s")){
                shmid = atoi(argv[i+1]);
            }
            else if(!strcmp(argv[i], "-n")){
                no_sldmker = atoi(argv[i+1]);
            }
            else{
                printf("Wrong execute of program!Try again\n");
                return 1;
            }
        }
    }
    else{
        printf("Wrong execute of program!Try again\n");
        return 1;
    }



    if(lb < 0 || ub < 0 || shmid < 0){
        printf("Wrong execute of program!Try again\n");
        return 1;
    }


    if(no_sldmker != 1 && no_sldmker != 2 && no_sldmker != 3){
        printf("Number of saladmaker should be 1,2 or 3.\n");
        return 1;
    }

    saladmaker(lb, ub, shmid, no_sldmker);


    return 0;
}
