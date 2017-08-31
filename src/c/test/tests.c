#include <BCUnit/BCUnit.h>
#include <BCUnit/Basic.h>
#include <stdbool.h>
#include "../utils/include/linkedlist.h"
#include "../utils/include/ticktimerhelper.h"

typedef struct {
	int (*f_ptr)(void);
} test_callback_ptr;

static int count = 0;

typedef void(*tt_ptr)(int);

void ll_data_helper(void *data, void *context) {	
	int expected = *(int*)data;
	CU_ASSERT_EQUAL(expected, 11);
}

void ll_data_helper2(void *data, void *context) {
	test_callback_ptr *container = data;
	int expected = container->f_ptr();
	CU_ASSERT_EQUAL(expected, 42);
}

int ll_test_helper(void) {
	return 42;
}


bool is_between_numbers(int begin, int end, int now) {
  bool within_24h = begin < end;
  if (within_24h) {    
    return (now > begin) && (now < end);
  } 
  return (((now < begin) && (now < end)) || ((now > begin) && (now > end)));
}

void test_is_between_numbers(void) {
	CU_ASSERT_TRUE(is_between_numbers(100, 200, 150));
	CU_ASSERT_FALSE(is_between_numbers(100, 200, 300));
	CU_ASSERT_FALSE(is_between_numbers(100, 200, 50));

	CU_ASSERT_TRUE(is_between_numbers(200, 100, 50));
	CU_ASSERT_TRUE(is_between_numbers(200, 100, 250));
	CU_ASSERT_FALSE(is_between_numbers(200, 100, 150));	
}

void test_linked_list(void)
 {
	LinkedList *ll = init_linkedlist();
	CU_ASSERT_TRUE(ll != NULL);
	CU_ASSERT_EQUAL(linkedlist_size(ll), 0);

	int data1 = 11;
	int data2 = 22;
	int data3 = 33;
	linkedlist_append(ll, &data1);
	CU_ASSERT_EQUAL(linkedlist_size(ll), 1);
	linkedlist_append(ll, &data2);
	linkedlist_append(ll, &data3);
	CU_ASSERT_EQUAL(linkedlist_size(ll), 3);

	linkedlist_clear(ll);
	CU_ASSERT_EQUAL(linkedlist_size(ll), 0);

	linkedlist_append(ll, &data1);
	linkedlist_forEachData(ll, ll_data_helper, NULL);

	test_callback_ptr tst = {
		.f_ptr = ll_test_helper
	};

	linkedlist_clear(ll);
	linkedlist_append(ll, &tst);
	linkedlist_forEachData(ll, ll_data_helper2, NULL);

	deinit_linkedlist(ll);
}

void tt_test_helper(int data) {
	CU_ASSERT_EQUAL(data, 42);	
}

void tt_test_deephelper(int data) {
	count = count + 1;	
}


void test_ticktimerhelper(void) {
	start_ticktimerhelper();
	ticktimerhandler p = tt_test_helper;
	ticktimerhelper_register(p);
	ticktimerhelper_emulate(42);	
	stop_ticktimerhelper();
}

void test_ticktimerhelper_deep(void) {
	start_ticktimerhelper();
	ticktimerhandler p = tt_test_deephelper;
	ticktimerhelper_register(p);
	ticktimerhelper_register(p);
	ticktimerhelper_register(p);
	ticktimerhelper_emulate(1);		
	CU_ASSERT_EQUAL(count, 3);
	stop_ticktimerhelper();
}

static int suite_success_init(void) { return 0; }
static int suite_success_clean(void) { return 0; }

static CU_TestInfo tests_success[] = {
  { "test_is_between_numbers", test_is_between_numbers },
  { "test_linked_list", test_linked_list },
  { "test_ticktimerhelper", test_ticktimerhelper },
  { "test_ticktimerhelper_deep", test_ticktimerhelper_deep },
	CU_TEST_INFO_NULL
};

static CU_SuiteInfo suites[] = {
	{"time and calendar", suite_success_init, suite_success_clean,  NULL, NULL, tests_success},
	CU_SUITE_INFO_NULL
};

int main() {
	CU_initialize_registry();
	CU_register_suites(suites);
	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_set_error_action(CUEA_IGNORE);
	CU_basic_run_tests();
	CU_cleanup_registry();
}