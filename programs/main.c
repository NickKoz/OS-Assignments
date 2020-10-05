#include "common_types.h"


int main(int argc, char* argv[]){

    char* inputfile = NULL;
    char* configfile = NULL;

    if(argc == 5){
        if(!strcmp(argv[1], "-i")){
            inputfile = strdup(argv[2]);
            configfile = strdup(argv[4]);
        }
        else if(!strcmp(argv[1], "-c")){
            inputfile = strdup(argv[4]);
            configfile = strdup(argv[2]);
        }
        else{
            printf("Wrong execute of program!Try again\n");
            return 1;
        }
    }
    else if(argc == 3){
        if(!strcmp(argv[1], "-i"))
            inputfile = strdup(argv[2]);
        else if(!strcmp(argv[1], "-c"))
            configfile = strdup(argv[2]);
        else{
            printf("Wrong execute of program!Try again\n");
            return 1;
        }
    }
    else if(argc > 5){
        printf("Wrong execute of program!Try again\n");
        return 1;
    }

    add_users_files_data(inputfile, configfile);

    prompt();

}

