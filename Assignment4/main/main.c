#include "common_types.h"



int main(int argc, char* argv[]){

    char *origindir = NULL, *destdir = NULL;
    
    // Checks arguments in any possible order.
    if(argc >= 3 && argc <= 6){
        for(int i = 1 ; i < argc ; i++){
            if(!strcmp(argv[i], "-v")){
                verbose = true;
            }
            else if(!strcmp(argv[i], "-d")){
                deleted = true;
            }
            else if(!strcmp(argv[i], "-l")){
                links = true;
            }
            else{
                if(argv[i][0] == '-'){
                    printf("Wrong execute of program!Try again!\n");
                    return 1;
                }
                if(origindir == NULL){
                    origindir = argv[i];
                    destdir = argv[i+1];
                }
            }
        }
    }
    else{
        printf("Wrong execute of program!Try again!\n");
        return 1;
    }

    // If origin dir does not exist, program ends.
    if(!check_if_dir(origindir)){
        printf("Origin dir is not valid!\n");
        return 1;
    }


    quic(origindir, destdir);

    printf("BYTES: %d\n", bytes_copied);
    printf("Time: %.6f\n", seconds_for_copy);

}
