#include "include/ticktimerhelper.h"
#include "include/linkedlist.h"

#if defined (TESTS)
#include <stdlib.h>
#else
#include <pebble.h>

static void tick_handler(struct tm *tick_time, TimeUnits units_changed);

#endif

typedef struct TimerHandlerContainer {
	ticktimerhandler tt_handler;
} TimerHandlerContainer;

static LinkedList *container;
static void prv_deinit_helper(void *, void*);

void start_ticktimerhelper() {	
	if (!container) {
		container = init_linkedlist();
#if !defined (TESTS)		
  	tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);		
#endif  		
	}
}

void stop_ticktimerhelper() {
	if (container) {
		linkedlist_forEachData(container, prv_deinit_helper, NULL);
		deinit_linkedlist(container);
	}
}

void ticktimerhelper_register(ticktimerhandler tth) {
	if (!container) {
		start_ticktimerhelper();
	}
	TimerHandlerContainer *helperstruct = malloc(sizeof(TimerHandlerContainer));	
	helperstruct->tt_handler = tth;
	linkedlist_append(container, helperstruct);
}

static void prv_deinit_helper(void *item, void* context) {
	if (item) {
		TimerHandlerContainer *thc = item;
		free(thc);
	}
}


#if defined (TESTS)
static void prv_ticktimerhelper_launcher(void *handler, void *context) {
	TimerHandlerContainer *thc = handler;
	ticktimerhandler func = thc->tt_handler;
	int *data = context;
	func(*data);
}

void ticktimerhelper_emulate(int num) {
	if (!container) {
		return;
	}
	linkedlist_forEachData(container, prv_ticktimerhelper_launcher, &num);
}
#else

static void prv_ticktimerhelper_launcher(void *handler, void *context) {

	TimerHandlerContainer *thc = handler;
	ticktimerhandler func = thc->tt_handler;
	struct tm * tick_time = context;
	func(tick_time);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
	if (units_changed == 0) {
		return;
	}	
	linkedlist_forEachData(container, prv_ticktimerhelper_launcher, tick_time);
}

#endif
