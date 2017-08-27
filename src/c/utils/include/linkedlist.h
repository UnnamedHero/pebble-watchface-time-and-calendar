#pragma once
#include <stdbool.h>

typedef enum LinkedListResult {
	LIST_OK,
	LIST_MALLOC_ERROR
} LINKED_LIST_RESULT;

typedef struct LinkedListItem LinkedListItem;

typedef struct LinkedList LinkedList;

typedef void(*ll_callback_ptr)(void *callback_data, void *context);

//LINKED_LIST_RESULT 
LinkedList* init_linkedlist();

//LINKED_LIST_RESULT
void deinit_linkedlist(LinkedList *ll_ptr);

//LINKED_LIST_RESULT 
bool linkedlist_append(LinkedList *ll_ptr, void *data);

//LINKED_LIST_RESULT linkedlist_delete(LinkedList *ll_ptr, int index);
//LINKED_LIST_RESULT 
void linkedlist_clear(LinkedList *ll_ptr);
int linkedlist_size(LinkedList *ll_ptr);

//LINKED_LIST_RESULT linkedlist_getFirst(LinkedList *ll_ptr, void *out_data_ptr);

//LINKED_LIST_RESULT 
void linkedlist_forEachData(LinkedList *ll_ptr, ll_callback_ptr, void *context);
