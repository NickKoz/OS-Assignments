#ifndef STRUCTURES
#define STRUCTURES

#include "common_types.h"
#include "HashTable.h"
#include "InvertedIndex.h"
#include "Student.h"
#include "Operations.h"

#define SIZE_OF_KEY sizeof(int*)

#define STRING_DEFAULT 255

#define HASH_TABLE_DEFAULT_SIZE 50 // default size of hash table if not given.

extern int string_size;
extern int hash_table_size;
extern HTable students;
extern InvIndex each_year_students;



#endif