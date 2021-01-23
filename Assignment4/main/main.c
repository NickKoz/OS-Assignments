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


    double bytes_pace;
    if(seconds_for_copy == 0.0){
        bytes_pace = 0.0;
    }
    else{
        bytes_pace = bytes_copied / seconds_for_copy;
    }

    printf("There are %d files/directories in the hierarchy\n", entities);
    printf("Number of entities copied is %d\n", entities_copied);
    printf("Copied %d bytes in %.6f sec at %.6f bytes/sec\n", bytes_copied, seconds_for_copy, bytes_pace);

}
