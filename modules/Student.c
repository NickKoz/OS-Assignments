#include "Student.h"


Student Student_create(char* given_line){

    char delim[2] = " ";

    char* id = strtok(given_line, delim);
    char* first_name = strtok(NULL, delim);
    char* last_name = strtok(NULL, delim);
    char* zip = strtok(NULL, delim);
    char* year = strtok(NULL, delim);
    char* gpa = strtok(NULL, delim);

    // If user hasn't given us some information, we abort operation.
    if(id == NULL || first_name == NULL || last_name == NULL 
    || zip == NULL || year == NULL || gpa == NULL){
        // we return NULL.
        return NULL;
    }


    // Else, we create the Student.
    Student S = malloc(sizeof(struct student));
    if(S == NULL){
        printf("Memory error!\n");
        assert(0);
    }

    S->student_id = strdup(id);
    S->first_name = strdup(first_name);
    S->last_name = strdup(last_name);
    S->zip = atoi(zip);
    S->year = atoi(year);
    S->gpa = atof(gpa);

    // If we are given something that we don't expect,
    if(S->student_id == NULL || S->first_name == NULL || S->last_name == NULL 
    || S->zip == 0 || S->year == 0 || S->gpa < 0.0){
        free(S->student_id);
        free(S->first_name);
        free(S->last_name);
        free(S);
        // we return NULL.
        return NULL;
    }

    return S;
}


void Student_print(Student S){

    printf("ID: %s\n", S->student_id);
    printf("First Name: %s\n", S->first_name);
    printf("Last Name: %s\n", S->last_name);
    printf("Postal Code: %d\n", S->zip);
    printf("Register Year : %d\n", S->year);
    printf("GPA: %.2f\n\n", S->gpa);
}


void Student_destroy(Student S){

    free(S->student_id);
    free(S->first_name);
    free(S->last_name);

    free(S);
}
