#ifndef _DATA_STRUCTURES_LINKED_LIST_
#define _DATA_STRUCTURES_LINKED_LIST_

#include "common_types.h"


typedef Pointer LLItem;

typedef struct LinkedListNode* LLNode;

typedef struct LinkedList* LList;



LLNode LLNode_create(void);
LLItem LLNode_get_data(LLNode);
void LLNode_set_data(LLNode, LLItem);
LLNode LLNode_next(LLNode);


LList LL_create(DestroyFunc);
bool LL_is_empty(LList);
LLNode LL_first(LList);
LLNode LL_last(LList);
int LL_size(LList);
void LL_set_destroy_func(LList,DestroyFunc);
DestroyFunc LL_get_destroy_func(LList);
void LL_insert_front(LList,LLItem);
void LL_insert_last(LList,LLItem);
LLNode LL_find(LList,LLItem,CompareFunc);
void LL_delete(LList,LLNode);
void LL_visit(LList,VisitFunc);
void LL_destroy(LList L);


#endif