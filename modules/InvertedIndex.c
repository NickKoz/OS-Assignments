#include "InvertedIndex.h"


/*
    * Node of Inverted Index structure. It can be referred to as a bucket with data 
    * that can be accessed by a key.
    * The key is common for all data in the bucket and bucket's data is references of existing objects.
*/
struct InvertedIndexNode{

    InvIndexKey key;
    LList data;

};

// Creates and returns a bucket with the given key.
InvIndexNode InvIndexNode_create(InvIndexKey key){

    InvIndexNode IINode = malloc(sizeof(struct InvertedIndexNode));
    if(IINode == NULL){
        printf("Memory error!\n");
        assert(0);
    }
    IINode->key = key;
    IINode->data = LL_create(NULL); // Destroy function is NULL because we just store references of objects.

    return IINode;
}

InvIndexKey InvIndexNode_get_key(InvIndexNode IINode){
    return IINode->key;
}

LList InvIndexNode_get_data(InvIndexNode IINode){
    return IINode->data;
}


void InvIndexNode_visit(InvIndexNode IINode, VisitFunc visitKey, VisitFunc visitData){
    
    visitKey(IINode->key);
    LL_visit(IINode->data, visitData);

}


// Destroys bucket and its collection.
void InvIndexNode_destroy(Pointer p){

    InvIndexNode IINode = p;

    LL_destroy(IINode->data);

    free(IINode);
}






/*
    * Inverted Index structure is a data structure for grouping a collection of data 
    * by keys. This is useful for finding information about the groups formed.
*/
struct InvertedIndex{

    LList buckets;
    CompareFunc compare;

};



/* 
    * Creates and returns a Inverted Index structure.
    * Compare function for the key is given.
*/
InvIndex InvIndex_create(CompareFunc cfunc){

    InvIndex I = malloc(sizeof(struct InvertedIndex));
    if(I == NULL){
        printf("Memory error!\n");
        assert(0);
    }

    I->buckets = LL_create(InvIndexNode_destroy);
    I->compare = cfunc;

    return I;
}


LList InvIndex_get_buckets(InvIndex I){
    assert(I != NULL);
    return I->buckets;
}


static int compare_indexes(Pointer p1, Pointer p2){
    return p2 - p1;
}


// Inserts item by key in structure.
void InvIndex_insert(InvIndex I, InvIndexKey key, InvIndexItem item){

    // Temporary bucket in order to search for key in buckets' list.
    InvIndexNode tempBucket = InvIndexNode_create(key);

    LLNode header_bucket = LL_find(I->buckets, tempBucket, I->compare);
    // If bucket with given key doesn't exist, we add it to our buckets' collection.
    if(header_bucket == NULL){
        LL_insert_last(I->buckets, tempBucket);
    }

    // We extract the bucket that we need.
    InvIndexNode bucket = LLNode_get_data(LL_find(I->buckets, tempBucket, I->compare));

    // We delete the temporary bucket if we didn't add it to buckets' list.
    if(header_bucket != NULL)
        InvIndexNode_destroy(tempBucket);

    // If item isn't already in bucket,
    if(LL_find(bucket->data, item, compare_indexes) == NULL)
        // we add it in it.
        LL_insert_front(bucket->data, item);

}




// Removes item with key from the structure. Returns true if item is removed or false if not.
void InvIndex_delete(InvIndex I, InvIndexKey key, InvIndexItem item){

    // Temporary bucket in order to search for key in buckets' list.
    InvIndexNode tempBucket = InvIndexNode_create(key);

    LLNode header_bucket = LL_find(I->buckets, tempBucket, I->compare);

    // We delete the temporary bucket because it is no more needed.
    InvIndexNode_destroy(tempBucket);

    // If bucket doesn't exist, we don't have something to do.
    if(header_bucket == NULL)
        return;

    // Else, we extract the bucket that we need.
    InvIndexNode bucket =  LLNode_get_data(header_bucket);

    LLNode data_to_be_deleted = LL_find(bucket->data, item, compare_indexes);

    // Finally we delete item.
    LL_delete(bucket->data, data_to_be_deleted);

    // If the bucket is empty after deleting item, we remove it from the structure
    // for space economy.
    if(LL_size(bucket->data) == 0)
        LL_delete(I->buckets, header_bucket);

}


// Finds bucket with given key and returns it. Returns NULL if it doesn't exist.
LList InvIndex_find(InvIndex I, InvIndexKey key){

    // Temporary bucket in order to search for key in buckets' list.
    InvIndexNode tempBucket = InvIndexNode_create(key);

    LLNode header_bucket = LL_find(I->buckets, tempBucket, I->compare);

    if(header_bucket == NULL)
        return NULL;

    InvIndexNode_destroy(tempBucket);

    return InvIndexNode_get_data(LLNode_get_data(header_bucket));   
}






void InvIndex_visit(InvIndex I, VisitFunc visitKey, VisitFunc visitData){

    for(LLNode temp = LL_first(I->buckets) ; temp != NULL ; temp = LLNode_next(temp))
        InvIndexNode_visit(LLNode_get_data(temp), visitKey, visitData);

}


// Destroys Inverted Index structure.
void InvIndex_destroy(InvIndex I){

    LL_destroy(I->buckets);

    free(I);
}
