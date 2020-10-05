#include "Structures.h"


void insert(char* data){
    
    Student S = Student_create(data);
    if(S == NULL){
        printf("Wrong insert operation call!Try again.\n");
        return;
    }

    if(HT_find(students, S->student_id)){
        printf("Student with ID: \'%s\' already exists!\n", S->student_id);
        return;
    }

    HT_insert(students, S->student_id, S);

    InvIndex_insert(each_year_students, &S->year, S);

    printf("Student is added to the system!\n");
}


void lookup(char* data){

    char* id = strtok(data, "\n");

    HTEntry stud = HT_find(students, id);

    // If student doesn't exist, we print a corresponding message.
    if(stud == NULL){
        printf("Student with ID: \'%s\' doesn't exist!\n",  id);
        return;
    }

    Student S = HTEntry_get_data(stud);
    // Else, we print student's attributes.
    Student_print(S);
}


void delete(char* data){

    char* id = strtok(data, "\n");

    HTEntry stud = HT_find(students, id);

    // If student doesn't exist, we print a corresponding message.
    if(stud == NULL){
        printf("Student with ID: \'%s\' doesn't exist!\n",  id);
        return;
    }

    Student S = HTEntry_get_data(stud);
    // Else, we delete him from the system.
    InvIndex_delete(each_year_students, &S->year, S);
    HT_remove(students, S->student_id);

    printf("Student with ID: \'%s\' is removed!\n",  id);
}


void number(char* data){

    int year = atoi(strtok(data, "\n"));

    if(year == 0){
        printf("Wrong year selection!Please try again.\n");
        return;
    }

    LList studs = InvIndex_find(each_year_students, &year);

    if(studs == NULL){
        printf("There are no students' records of year %d.\n", year);
        return;
    }

    printf("Number of students in year %d : %d\n", year, LL_size(studs));
}



int cmp_gpas(const void* p1, const void* p2){

    Student S1 = *(Student*)p1;
    Student S2 = *(Student*)p2;

    return (S1->gpa < S2->gpa) - (S1->gpa > S2->gpa);
}



void top(char* data){

    int stud_number = atoi(strtok(data, " "));
    int year = atoi(strtok(NULL, "\n"));

    // Case that user has given wrong year or student number.
    if(stud_number == 0 || year == 0){
        printf("Wrong year or number of students input!Please try again.\n");
        return;
    }
    //Case that there is no bucket with students in given year.
    LList studs = InvIndex_find(each_year_students, &year);
    if(studs == NULL){
        printf("There are no students' records of year %d.\n", year);
        return;
    }

    int studs_found = LL_size(studs);

    // Case that user gives wrong values.
    if(stud_number > studs_found || stud_number <= 0){
        printf("There are %d students in year %d.Try again.\n", studs_found, year);
        return;
    }

    // Array with references to Students in order to sort them by their gpa.
    Student students_by_gpa[studs_found];

    LLNode temp = LL_first(studs);
    for(int i = 0 ; i < studs_found ; i++){
        Student S = LLNode_get_data(temp);
        students_by_gpa[i] = S;
        temp = LLNode_next(temp);
    }
    // Sorting the students by gpa.
    qsort(students_by_gpa, studs_found, sizeof(Student), cmp_gpas);

    if(stud_number > 1)
        printf("The top %d students with the best GPA in year %d are: \n\n", stud_number, year);
    else
        printf("The student with the best GPA in year %d is: \n\n", year);
    // Finally we print the students.
    for(int i = 0 ; i < stud_number ; i++){
        Student_print(students_by_gpa[i]);
    }
}


void average(char* data){

    int year = atoi(strtok(data, "\n"));

    if(year == 0){
        printf("Wrong year selection!Please try again.\n");
        return;
    }

    LList studs = InvIndex_find(each_year_students, &year);
    if(studs == NULL){
        printf("There are no students' records of year %d.\n", year);
        return;
    }

    float sum_of_gpas = 0.0;

    int studs_found = LL_size(studs);

    for(LLNode temp = LL_first(studs) ; temp != NULL ; temp = LLNode_next(temp)){
        Student S = LLNode_get_data(temp);
        sum_of_gpas += S->gpa;
    }

    float average = sum_of_gpas/((float)studs_found);

    printf("Average GPA for students of year %d is: %.2f \n\n", year, average);
}


void minimum(char* data){

    int year = atoi(strtok(data, "\n"));

    if(year == 0){
        printf("Wrong year selection!Please try again.\n");
        return;
    }

    LList studs = InvIndex_find(each_year_students, &year);
    if(studs == NULL){
        printf("There are no students' records of year %d.\n", year);
        return;
    }

    float min_gpa = 101.0;
    for(LLNode temp = LL_first(studs) ; temp != NULL ; temp = LLNode_next(temp)){
        Student S = LLNode_get_data(temp);
        if(S->gpa < min_gpa){
            min_gpa = S->gpa;
        }
    }
    // Many students may have lowest GPA of given year.
    printf("Student/Students that had the lowest GPA is/are:\n\n");
    // So we print them all.
    for(LLNode temp = LL_first(studs) ; temp != NULL ; temp = LLNode_next(temp)){
        Student S = LLNode_get_data(temp);
        if(S->gpa == min_gpa)
            Student_print(S);
    }
}


void count(char* data){

    LList buckets_with_years = InvIndex_get_buckets(each_year_students);

    if(LL_is_empty(buckets_with_years)){
        printf("No students are enrolled\n\n");
        return;
    }

    // For each year,
    for(LLNode temp = LL_first(buckets_with_years) ; temp != NULL ; temp = LLNode_next(temp)){

        InvIndexNode header_bucket = LLNode_get_data(temp);

        int year = *(int*)InvIndexNode_get_key(header_bucket);
        int students = LL_size(InvIndexNode_get_data(header_bucket));

        printf("Year: %d  Students: %d\n", year, students);
    }
    printf("\nAll the other possible years don't have active students.\n\n");
}







typedef struct ZipInfo{

    int zipcode;
    int num_of_students;

}ZipInfo;


int cmp_zipcodes(const void* p1, const void* p2){

    ZipInfo* z1 = *(ZipInfo**)p1;
    ZipInfo* z2 = *(ZipInfo**)p2;

    return z2->num_of_students - z1->num_of_students;
}


void postal(char* data){

    int rank = atoi(strtok(data, "\n"));

    if(rank == 0 || rank < 0){
        printf("Wrong rank selection!Please try again.\n");
        return;
    }

    LList buckets_with_years = InvIndex_get_buckets(each_year_students);

    if(LL_is_empty(buckets_with_years)){
        printf("No students are enrolled\n\n");
        return;
    }

    int number_of_students = HT_count(students);
    
    ZipInfo* zip_struct[number_of_students];

    for(int i = 0 ; i < number_of_students ; i++){
        zip_struct[i] = malloc(sizeof(ZipInfo));
        zip_struct[i]->zipcode = -1;
        zip_struct[i]->num_of_students = 0;
    }

    int number_of_zipcodes = 0;

    int zip_end_index = 0;
    for(LLNode temp = LL_first(buckets_with_years) ; temp != NULL ; temp = LLNode_next(temp)){

        InvIndexNode header_bucket = LLNode_get_data(temp);
        LList studs = InvIndexNode_get_data(header_bucket);

        for(LLNode temp_stud = LL_first(studs) ; temp_stud != NULL ; temp_stud = LLNode_next(temp_stud)){
            Student S = LLNode_get_data(temp_stud);
            bool found = false;
            for(int i = 0 ; i < zip_end_index ; i++){
                if(S->zip == zip_struct[i]->zipcode){
                    zip_struct[i]->num_of_students++;
                    found = true;
                    break;
                }
            }
            if(!found){
                number_of_zipcodes++;
                zip_struct[zip_end_index]->num_of_students++;
                zip_struct[zip_end_index++]->zipcode = S->zip;
            }
        }
    }


    if(rank > number_of_zipcodes){
        printf("Wrong rank selection!Please try again.\n");
        return;
    }
    
    qsort(zip_struct, number_of_students, sizeof(ZipInfo*), cmp_zipcodes);

    int curr_zip = zip_struct[0]->zipcode;
    for(int i = 1 ; i < number_of_zipcodes + 1 ; i++){
        if(rank == 1)
            printf("%d ", curr_zip);

        if(zip_struct[i]->zipcode != curr_zip){
            if(rank > 1){
                curr_zip = zip_struct[i]->zipcode;
                rank--;
            }
            else if(rank == 1){
                break;
            }
        }
    }

    printf("is/are %d most popular.\n", rank);

    for(int i = 0 ; i < number_of_students ; i++){
        free(zip_struct[i]);
    }
}




void exit_program(){
    InvIndex_destroy(each_year_students);
    HT_destroy(students);
}
