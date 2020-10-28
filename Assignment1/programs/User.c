#include "Structures.h"


int hash_string(HTKey key){

    int hash=0, a=33;
    char* temp = key;

    for(; *temp!='\0' ; temp++)
        hash = (a*hash + *temp);

    return hash;
}

void visit(Pointer p){
    printf("(%s)", (char*)HTEntry_get_key(p));
}

int HT_compare(Pointer p1, Pointer p2){
    return strcmp((char*)HTEntry_get_key(p1), (char*)HTEntry_get_key(p2));
}

int InvIndex_compare(Pointer p1, Pointer p2){
    return *(int*)InvIndexNode_get_key(p2) - *(int*)InvIndexNode_get_key(p1);
}

void destroy_student(Pointer p){
    Student_destroy(HTEntry_get_data(p));
    // Destroying list node's data.
    HTEntry_destroy(p);
}



// Returns lines' number of a file.
int file_lines(FILE* fp){   

    int line_counter = 0;
    char ch;

    while(feof(fp) == 0){
        // We read each character.
        ch = fgetc(fp);
        // If we reach \n, we have a line.
        if(ch == '\n')
            line_counter++;
    }
    // Resets file pointer to the start of the file.
    rewind(fp);
    return line_counter;
}


void initialize_structures(){
    students = HT_create(hash_table_size, HT_compare, hash_string, destroy_student);
    each_year_students = InvIndex_create(InvIndex_compare);
}



// Reads input file and stores its data.
void read_input_file(char* filename){

    FILE* input_file = fopen(filename, "r");
     // If we can't open the file, we stop program.
    if(input_file == NULL){
        printf("Input file not found!\n");
        assert(0);
    }
    
    int number_of_students = file_lines(input_file);

    // If configuration file hasn't changed hash table's size,
    // we set hash table's size same as the number of students given.
    if(hash_table_size == HASH_TABLE_DEFAULT_SIZE)
        hash_table_size = number_of_students;

    initialize_structures();

    // A buffer used for fetching each line of file.
    char buffer_line[string_size];

    // Number of student duplicates that input file may have.
    int number_of_duplicates = 0;

    for(int i = 0 ; i < number_of_students ; i++){

        // We fetch each data line of file.
        fgets(buffer_line, string_size, input_file);

        // Student to be inserted into structure.
        Student S = Student_create(buffer_line);
        if(S == NULL){
            printf("Error in student data in %dth row.\n", i+1);
            assert(0);
        }

        // If student is not in hash table,
        if(HT_find(students, S->student_id) == NULL){
            // we insert them into both hash table
            HT_insert(students, S->student_id, S);
            // and inverted index structure.
            InvIndex_insert(each_year_students, &S->year, SIZE_OF_KEY, S);
        }
        else{
            // Else, we delete student that we created before.
            Student_destroy(S);
            number_of_duplicates++;
        }
        
    }

    if(number_of_duplicates){
        printf("%d duplicated records found and not inserted!\n", number_of_duplicates);
    }

    fclose(input_file);
    free(filename);
}



// Reads configuration file and changes some settings.
void read_config_file(char* filename){

    FILE* config_file = fopen(filename, "r");
     // If we can't open the file, we stop program.
    if(config_file == NULL){
        printf("Configuration file not found!\n");
        assert(0);
    }

    int number_of_lines = file_lines(config_file);

    char buffer_line[string_size];
    char delim[2] = " ";

    for(int i = 0 ; i < number_of_lines ; i++){

        // Reading each line of configuration file.
        fgets(buffer_line, string_size, config_file);

        // Tokenize each line in order to fetch data required.
        char* token = strtok(buffer_line, delim);

        // User has 2 options: Change hash table size or string size of a line in input file.
        // Code below covers all fail user's input cases.

        if(!strcmp(token, "HASH_TABLE_SIZE") ){
            token = strtok(NULL, delim);
            if(!strcmp(token, "=")){
                token = strtok(NULL, delim);
                hash_table_size = atoi(token);
                if(hash_table_size == 0){
                    printf("Wrong hash table size! Check configuration file's format.\n");
                    assert(0);
                }
            }
            else{
                printf("Wrong hash table size format! Check configuration file's format.\n");
                assert(0);
            }
        }
        else if(!strcmp(token, "STRING_SIZE")){
            token = strtok(NULL, delim);
            if(!strcmp(token, "=")){
                token = strtok(NULL, delim);
                string_size = atoi(token);
                if(string_size == 0){
                    printf("Wrong string size! Check configuration file's format.\n");
                    assert(0);
                }
            }
            else{
                printf("Wrong string size format! Check configuration file's format.\n");
                assert(0);
            }
        }
        else if(!strcmp(token, "\n")){
            continue;
        }
        else{
            printf("Unknown variable in configuration file. Check again documentation.\n");
            assert(0);
        }
    }

    fclose(config_file);
    free(filename);
}




void add_users_files_data(char* inputfile, char* configfile){

    // If we don't have a file, we should initialize structures.
    if(inputfile == NULL && configfile == NULL){
        printf("No input file or configuration file.\n");
        printf("Hash table size is set to default: %d\n", hash_table_size);
        initialize_structures();
        return;
    }
    
    // Case that we have only input file.
    if(inputfile != NULL && configfile == NULL){
        read_input_file(inputfile);
    }
    // Case that we have only configuration file.
    else if(inputfile == NULL && configfile != NULL){
        read_config_file(configfile);
        initialize_structures();
    }
    // Case that we have both.
    else{
        read_config_file(configfile);
        read_input_file(inputfile);
    }

}
