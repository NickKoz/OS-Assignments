#include "List.h"


/* 
    * Node of linked list. Its data is void pointer in order to 
    * be of any type. Also, node has a reference to the next node in list.
*/
struct LinkedListNode{

    LLItem data;
    LLNode link;
    
};

//Returns data of node.
LLItem LLNode_get_data(LLNode node){
    assert(node != NULL);
    return node->data;
}


//Sets node's data.
void LLNode_set_data(LLNode node, LLItem item){
    assert(node != NULL);
    node->data = item;
}


//Returns next of node.
LLNode LLNode_next(LLNode node){
    assert(node != NULL);
    return node->link;
}




/* 
    * Linked List (LList). 
    * It's a structure that supports insertion, deletion and lookup in constant time.
    * 
    * Contains: 
    * 1. head that points to the first node of the list.
    * 2. tail that points to the last node of the list.
    * 3. size of list.
    * 4. destroy function that is responsible to delete LLNodes' data.
*/
struct LinkedList{

    LLNode head;
    LLNode tail;
    int size;
    DestroyFunc destroy_value;

};



/*
    * Creates generic linked list. Initializes members of
    * list structure. Gets a destroy function for deleting LLNodes' data by argument.
    * Returns the new list.
*/
LList LL_create(DestroyFunc dfunc){

    LList L = malloc(sizeof(struct LinkedList));
    if(L == NULL){
        printf("Memory error!\n");
        return NULL;
    }
    L->head = NULL;
    L->tail = NULL;
    L->size = 0;
    L->destroy_value = dfunc;
    return L;
}


//Checks if list is empty. Returns true or false.
bool LL_is_empty(LList L){
    return L->size != 0 ? false : true;
}


//Returns first node of list.
LLNode LL_first(LList L){
    return L->head;
}


//Returns last node of list.
LLNode LL_last(LList L){
    return L->tail;
}


int LL_size(LList L){
    return L->size;
}


//Sets destroy function for LLNodes of list to be dfunc.
void LL_set_destroy_func(LList L, DestroyFunc dfunc){
    L->destroy_value = dfunc;
}


DestroyFunc LL_get_destroy_func(LList L){
    return L->destroy_value;
}


//Inserts new item in front of list. Takes O(1) time.
void LL_insert_front(LList L, LLItem item){

    LLNode temp = LLNode_create();
    if(temp == NULL){
        printf("Memory error!\n");
        return;
    }

    temp->data = item;

    temp->link = L->head;
    L->head = temp;

    if(LL_is_empty(L))
        L->tail = temp;

    L->size++;
}

 
//Inserts new item at end of list. Takes O(1) time.
void LL_insert_last(LList L, LLItem item){

    LLNode temp = LLNode_create();
    if(temp == NULL){
        printf("Memory error!\n");
        return;
    }

    temp->data = malloc(sizeof(PMessage));
    ((PMessage*)temp->data)->prime_number = ((PMessage*)item)->prime_number;
    ((PMessage*)temp->data)->time_taken = ((PMessage*)item)->time_taken;
    temp->link = NULL;

    if(LL_is_empty(L)){
        L->head = temp;
        L->tail = temp;
    }
    else{
        L->tail->link = temp;
        L->tail = temp;
    }

    L->size++;
}

/* 
    * Finds item in list.
    * For this operation, it uses compare function given by argument.
    * Time needed : O(n)
*/
LLNode LL_find(LList L, LLItem item, CompareFunc compare){

    if(compare == NULL){
        printf("No compare function is given!\n");
        return NULL;
    }

    LLNode temp;
	temp = L->head;
	while(temp != NULL){
		if(!compare(temp->data,item))
			return temp;
		else
			temp=temp -> link;
	}
	
	return NULL;
}


//Deletes node of list. Worst case's time complexity is O(n). 
void LL_delete(LList L, LLNode ptrToNode){

    if(!L->size || ptrToNode == NULL)
        return;

    LLNode curr,prev,temp;
    // If head is about to be deleted, we delete it in O(1) time.
    if(L->head == ptrToNode){
        temp = L->head->link;
        if(L->destroy_value!=NULL)
            L->destroy_value(L->head->data);
        free(L->head);
        L->head = temp;
        L->size--;
        return;
    }
    // In other case, it takes O(n) time.
    curr = L->head;
    while(curr!=NULL){
        prev = curr;
        curr = curr->link;
        if(curr == ptrToNode){
            prev->link = curr->link;
            if(L->destroy_value != NULL)
                L->destroy_value(curr->data);
            free(curr);
            curr = prev->link;
            break;
        }
    }

    L->size--;

}


/*
    * Visits list.
    * For this operation, it uses visit function given by arguement.

*/
void LL_visit(LList L, VisitFunc visit){

    if(visit == NULL){
        printf("No visit function is given!\n");
        return;
    }

    if(!L->size){
        printf("The list is empty!\n");
        return;
    }

    LLNode temp = L->head;
	
	printf("[");
	while(temp != NULL){
		visit(temp->data);
		temp = temp->link;
		if(temp != NULL)
			printf(",");
	}
	printf("]\n");

}


// Destroys list using list's destroy function for LLNodes' data.
void LL_destroy(LList L){
	
	LLNode prev,curr = L->head;
	
	while(curr!=NULL){	
		prev = curr;
		curr = curr->link;
        if(L->destroy_value != NULL)
		    L->destroy_value(prev->data);
        free(prev);
	}

	free(L);
}


// Making new node.
LLNode LLNode_create(){
    return malloc(sizeof(struct LinkedListNode));
}

