#ifndef _INVERTED_INDEX_
#define _INVERTED_INDEX_

#include "List.h"

typedef struct InvertedIndex* InvIndex;

typedef struct InvertedIndexNode* InvIndexNode;

// Key that each bucket has.
typedef Pointer InvIndexKey;

// Item's reference that will be stored in a bucket.
typedef Pointer InvIndexItem;

InvIndexKey InvIndexNode_get_key(InvIndexNode);
LList InvIndexNode_get_data(InvIndexNode);

InvIndex InvIndex_create(CompareFunc);
LList InvIndex_get_buckets(InvIndex);
void InvIndex_insert(InvIndex, InvIndexKey, InvIndexItem);
void InvIndex_delete(InvIndex, InvIndexKey, InvIndexItem);
LList InvIndex_find(InvIndex, InvIndexKey);

void InvIndex_visit(InvIndex, VisitFunc, VisitFunc);

void InvIndex_destroy(InvIndex);

#endif