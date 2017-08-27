#include "include/linkedlist.h"

#if defined (TESTS)
#include <stdbool.h>
#include <stdlib.h>
#else
#include <pebble.h>
#endif

typedef struct LinkedListItem {
	LinkedListItem *next;
	void *data;
} LinkedListItem;

typedef struct LinkedList {
	LinkedListItem *first;
	LinkedListItem *last;
	int size;
} LinkedList;


static void prv_ll_fill_default_values(LinkedList *ll);
static void prv_linkedlist_remove_item(LinkedListItem *);

LinkedList* init_linkedlist() {
	LinkedList *ll = malloc(sizeof(LinkedList));
	prv_ll_fill_default_values(ll);
	return ll;
}


void deinit_linkedlist(LinkedList *ll) {
	linkedlist_clear(ll);
	free(ll);
}

bool linkedlist_append(LinkedList *ll, void *data) {
	if (!ll) {
		return false;
	}
	LinkedListItem *item = malloc(sizeof(LinkedListItem));
	if (!item) {
		return false;
	}
	item->next = NULL;
	item->data = data;

	if (ll->size == 0) {
		ll->first = item;
	}
	LinkedListItem *oldLast = ll->last;
	if (oldLast) {
		oldLast->next = item;	
	}
	ll->last = item;
	ll->size++;
	return true;
}

// void linkedlist_delete(LinkedList *ll_ptr, int index) {

// }
int linkedlist_size(LinkedList *ll) {
	if (!ll) {
		return -1;
	}
	return ll->size;

}

void linkedlist_clear(LinkedList *ll) {
	if(!ll) {
		return;
	}
	//linkedlist_forEach(ll, prv_linkedlist_remove_item);
	LinkedListItem *current = ll->first;

	while (current) {
		LinkedListItem *oldCurrent = current;				
		current = current->next;
		free(oldCurrent);
	}
	prv_ll_fill_default_values(ll);
}

//LINKED_LIST_RESULT linkedlist_getFirst(LinkedList *ll_ptr, void *out_data_ptr);

void linkedlist_forEachData(LinkedList *ll, ll_callback_ptr func, void *context) {
	if (ll->size < 0) {
		return;
	}
	LinkedListItem *current = ll->first;

	while (current) {
		func(current->data, context);
		current = current->next;
	}
}

static void prv_ll_fill_default_values(LinkedList *ll) {
	ll->first = NULL;
	ll->last = NULL;
	ll->size = 0;

}

static void prv_linkedlist_remove_item(LinkedListItem *item) {
	if (item) {
		item->next = NULL;
		item->data = NULL;
		free(item);
	}
}
