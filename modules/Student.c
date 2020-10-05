#include "Student.h"


Student Student_create(char* given_line){

    char delim[2] = " ";

    char* id = strdup(strtok(given_line, delim));
    char* first_name = strdup(strtok(NULL, delim));
    char* last_name = strdup(strtok(NULL, delim));
    int zip = atoi(strtok(NULL, delim));
    int year = atoi(strtok(NULL, delim));
    float gpa = atof(strtok(NULL, delim));

    // If we are given something that we don't expect,
    if(id == NULL || first_name == NULL || last_name == NULL 
    || zip == 0 || year == 0 ){ // gpa can be 0.0 so we don't include it.
        // we return NULL.
        return NULL;
    }

    // Else, we create the Student.
    Student S = malloc(sizeof(struct student));
    if(S == NULL){
        printf("Memory error!\n");
        assert(0);
    }

    S->student_id = id;
    S->first_name = first_name;
    S->last_name = last_name;
    S->zip = zip;
    S->year = year;
    S->gpa = gpa;

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
