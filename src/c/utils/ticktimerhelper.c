#include "include/ticktimerhelper.h"
#include "include/linkedlist.h"

#if defined (TESTS)
#include <stdlib.h>
#else
#include <pebble.h>
#include "../settings.h"

static void tick_handler(struct tm *tick_time, TimeUnits units_changed);
#endif

typedef struct TimerHandlerContainer {
	ticktimerhandler tt_handler;	
} TimerHandlerContainer;

static LinkedList *container = NULL;
static TimerHandlerContainer clock;
static void prv_deinit_helper(void *, void*);



void start_ticktimerhelper() {
	if (!container) {
		container = init_linkedlist();
#if !defined (TESTS)
	clock.tt_handler = NULL;
  	reload_timer();
#endif  		
	}
}


void reload_timer() {
	TimeUnits units = settings_get_ClockShowSeconds() == SEC_SHOWING ? SECOND_UNIT : MINUTE_UNIT;	
	tick_timer_service_subscribe(units, tick_handler);		
}

void stop_ticktimerhelper() {
	if (container) {
		linkedlist_forEachData(container, prv_deinit_helper, NULL);
		deinit_linkedlist(container);
		container = NULL;
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

void ticktimerhelper_register_clock(ticktimerhandler tth) {
	clock.tt_handler = tth;
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
	#if defined (DEBUG)
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Processing item handler.");
	#endif
	func(tick_time);	
}


static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
	#if defined (DEBUG)
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Tick handler called");
	#endif

	if (clock.tt_handler) {
		clock.tt_handler(tick_time);
	}
	
	if (units_changed == 0) {
		return;
	}

	// if (clock.tt_handler) {
	// 	clock.tt_handler(tick_time);
	// }

	if ((units_changed & MINUTE_UNIT) == MINUTE_UNIT) {
		linkedlist_forEachData(container, prv_ticktimerhelper_launcher, tick_time);
	}
}

#endif
