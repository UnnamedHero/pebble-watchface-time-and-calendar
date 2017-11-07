#pragma once
#include <stdbool.h>

typedef struct LinkedListItem LinkedListItem;

typedef struct LinkedList LinkedList;

typedef void(*ll_callback_ptr)(void *callback_data, void *context);
typedef void(*ll_callback_ptr_units)(void *callback_data, void *context, int);

LinkedList* init_linkedlist();

void deinit_linkedlist(LinkedList *ll_ptr);

bool linkedlist_append(LinkedList *ll_ptr, void *data);

void linkedlist_clear(LinkedList *ll_ptr);
int linkedlist_size(LinkedList *ll_ptr);


void linkedlist_forEachData(LinkedList *ll_ptr, ll_callback_ptr, void *context);
