#include "HashTable.h"


// Each hash table entry has a key and some data.
struct HashTableEntry{

    HTKey key;
    HTItem data;

};

static HTEntry HTEntry_create(HTKey key, HTItem item){

    HTEntry entry = malloc(sizeof(struct HashTableEntry));
    if(entry == NULL){
        printf("Memory error!\n");
        return NULL;
    }
    entry->key = key;
    entry->data = item;

    return entry;
}

// Setters and getters

void HTEntry_set_key(HTEntry entry , HTKey key){
    entry->key = key;
}

void HTEntry_set_data(HTEntry entry, HTItem item){
    entry->data = item;
}

HTKey HTEntry_get_key(HTEntry entry){
    return entry->key;
}

HTItem HTEntry_get_data(HTEntry entry){
    return entry->data;
}

// Destroys entry.
void HTEntry_destroy(HTEntry entry){
    free(entry);
}


/*
    * Hash Table (HTable) is a data structure that can map keys and values. It uses a hash function that computes an index
    * in an array of slots from which the desired value can be found.
    * This data structure provides search/insert/delete operations in constant time O(1) (average case).
*/
struct HashTable{

    LList* table; // The array of linked lists.
    int size;
    int count;
    CompareFunc compare;
    HashFunc hash;

};





/* 
  * Creates Hash Table. User has to input size of table, compare function 
  * in order to find a key, a hash function and a destroy function. 
  * (Destroy function has to destroy hash table entry too.)
*/
HTable HT_create(int size_of_table , CompareFunc cfunc, HashFunc hfunc, DestroyFunc dfunc){

    HTable H = malloc(sizeof(struct HashTable));
    if(H == NULL){
        printf("Memory error!\n");
        return NULL;
    }

    H->table = malloc(size_of_table * sizeof(LList));

    for(int i=0 ; i < size_of_table ; i++)
        H->table[i] = LL_create(dfunc);

    H->size = size_of_table;
    H->count = 0;
    H->compare = cfunc;
    H->hash = hfunc;

    return H;
}




// Inserts node into Hash Table with key and its data.
void HT_insert(HTable H, HTKey key, HTItem item){

    HTEntry tempEntry = HTEntry_create(key, item);

    int h = H->hash(key) % H->size;

    LL_insert_front(H->table[h], tempEntry);
    H->count++;
}


// Removes key from hash table.
void HT_remove(HTable H, HTKey key){

    HTEntry tempEntry = HTEntry_create(key, NULL);

    int h = H->hash(key) % H->size;
    // Searches for key in appropriate list.
    LLNode temp = LL_find(H->table[h], tempEntry, H->compare);
    if(temp != NULL){
        // If it's found, we delete it.
        LL_delete(H->table[h], temp);
        H->count--;
    }
    free(tempEntry);
}


// Finds key in hash table. Returns hash table entry or NULL if not found.
HTEntry HT_find(HTable H, HTKey key){

    HTEntry tempEntry = HTEntry_create(key, NULL);

    int h = H->hash(key) % H->size;
    // Searches for key in appropriate list.
    LLNode temp = LL_find(H->table[h], tempEntry, H->compare);

    free(tempEntry);
    // If found, we return hash table entry or, in other case NULL.
    return temp != NULL ? LLNode_get_data(temp) : NULL;

}


// Visits hash table using visit function.
void HT_visit(HTable H, VisitFunc visit){
    for(int i = 0 ; i < H->size ; i++){
        printf("Position %d: ", i);
        LL_visit(H->table[i], visit);
    }
}




void HT_set_destroy_func(HTable H, DestroyFunc dfunc){
    for(int i = 0 ; i < H->size ; i++)
        LL_set_destroy_func(H->table[i], dfunc);
}


DestroyFunc HT_get_destroy_func(HTable H){
    return LL_get_destroy_func(H->table[0]);
}


void HT_set_hash_func(HTable H, HashFunc hfunc){
    H->hash = hfunc;
}


HashFunc HT_get_hash_func(HTable H){
    return H->hash;
}

int HT_count(HTable H){
    return H->size;
}


// Destroys Hash Table. Frees all allocated memory.
void HT_destroy(HTable H){

    for(int i = 0 ; i < H->size ; i++)
        LL_destroy(H->table[i]);

    free(H->table);
    free(H);
}
