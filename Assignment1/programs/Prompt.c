#include "Operations.h"
#include <stdio.h>
#include <string.h>


void prompt(){

    // Command that user gives to our command prompt.
    char cmd[255];

    do{
        printf("> ");
        // Takes input from the standard input.
        fgets(cmd, 255, stdin);

        // We fetch the operation token and its data.
        char* operation = strtok(cmd, " ");
        char* data = strtok(NULL, "\n");
        char* diff_operation = strtok(cmd, "\n"); // for those that don't have arguments. (count, exit)

        if(!strcmp(operation, "\n"))
            continue;


        if(!strcmp(operation, "i")){
            insert(data);
        }
        else if(!strcmp(operation, "l")){
            lookup(data);
        }
        else if(!strcmp(operation, "d")){
            delete(data);
        }
        else if(!strcmp(operation, "n")){
            number(data);
        }
        else if(!strcmp(operation, "t")){
            top(data);
        }
        else if(!strcmp(operation, "a")){
            average(data);
        }
        else if(!strcmp(operation, "m")){
            minimum(data);
        }
        else if(!strcmp(diff_operation, "c")){
            count(data);
        }
        else if(!strcmp(operation, "p")){
            postal(data);
        }
        else if(!strcmp(diff_operation, "exit")){
            exit_program();
        }
        else{
            printf("Wrong operation!Please check the manual instructions.\n");
        }

        printf("\n");

    }while(strcmp(cmd, "exit"));
    
}
