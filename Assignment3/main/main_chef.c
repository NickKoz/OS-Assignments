#include "chef.h"


int main(int argc, char* argv[]){

    int numofSlds;
    double mantime;

    if(argc == 5){
        if(!strcmp(argv[1], "-n")){
            numofSlds = atoi(argv[2]);
            mantime = atof(argv[4]);
        }
        else if(!strcmp(argv[1], "-m")){
            numofSlds = atoi(argv[4]);
            mantime = atof(argv[2]);
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


    if(numofSlds <= 0){
        printf("Wrong number of salads!\n");
        return 1;
    }
    if(mantime <= 0.0){
        printf("Wrong mantime!\n");
        return 1;
    }


    chef(numofSlds, mantime);


}
