#ifndef _DATA_STRUCTURES_HASH_TABLE
#define _DATA_STRUCTURES_HASH_TABLE

// We're using List module for separate chaining implementation.
#include "List.h"

typedef Pointer HTItem;

typedef Pointer HTKey;

typedef struct HashTableEntry* HTEntry;

typedef struct HashTable* HTable;

typedef int (*HashFunc)(HTKey);

void HTEntry_set_key(HTEntry, HTKey);
void HTEntry_set_data(HTEntry, HTItem);
HTKey HTEntry_get_key(HTEntry);
HTItem HTEntry_get_data(HTEntry);
void HTEntry_destroy(HTEntry);


HTable HT_create(int, CompareFunc, HashFunc, DestroyFunc);
void HT_insert(HTable, HTKey, HTItem);
void HT_remove(HTable, HTKey);
HTEntry HT_find(HTable, HTKey);
void HT_visit(HTable , VisitFunc);
void HT_set_destroy_func(HTable, DestroyFunc);
DestroyFunc HT_get_destroy_func(HTable);
void HT_set_hash_func(HTable, HashFunc);
HashFunc HT_get_hash_func(HTable);
int HT_count(HTable);
void HT_destroy(HTable);


#endif