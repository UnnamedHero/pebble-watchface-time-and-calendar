#include <BCUnit/BCUnit.h>
#include <BCUnit/Basic.h>
#include <stdbool.h>

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

static int suite_success_init(void) { return 0; }
static int suite_success_clean(void) { return 0; }

static CU_TestInfo tests_success[] = {
  { "test_is_between_numbers", test_is_between_numbers },
	CU_TEST_INFO_NULL
};

static CU_SuiteInfo suites[] = {
	{"timeutils_tests", suite_success_init, suite_success_clean,  NULL, NULL, tests_success},
	CU_SUITE_INFO_NULL
};

int main() {
	CU_initialize_registry();
	CU_pSuite pSuite;
	pSuite = CU_add_suite("timeutils_tests", suite_success_init, suite_success_clean);
	//CU_add_test(pSuite, "test_is_between_numbers", test_is_between_numbers);
	CU_register_suites(suites);
	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_set_error_action(CUEA_IGNORE);
	CU_basic_run_tests();
	CU_cleanup_registry();
}