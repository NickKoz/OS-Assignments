#ifndef STUDENT
#define STUDENT

#include "common_types.h"

struct student{

    char* student_id;
    char* last_name;
    char* first_name;
    int zip;
    int year;
    float gpa;

};

typedef struct student* Student;


Student Student_create(char*);
void Student_print(Student);
void Student_destroy(Student);



#endif