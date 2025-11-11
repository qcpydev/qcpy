#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <test_common.h>

run_test_t run_test;

run_suite_t *run_tests_new_suite(Suite *suite) {
  if (!suite) {
    assert(0);
  }

  run_suite_t *new_test_suite = (run_suite_t *)malloc(sizeof(run_suite_t));

  if (!new_test_suite) {
    assert(0);
  }

  memset(new_test_suite, 0, sizeof(run_suite_t));
  new_test_suite->test_suite = suite;

  return new_test_suite;
}

bool run_tests_add_suite(Suite *suite) {
  if (!suite) {
    assert(0);
  }
  ++run_test.number_of_suites;
  if (!run_test.suites) {
    run_test.suites = run_tests_new_suite(suite);
    run_test.last_suite = run_test.suites;
  } else {
    run_test.last_suite->next_suite = run_tests_new_suite(suite);
    run_test.last_suite = run_test.last_suite->next_suite;
  }

  return true;
}

uint64_t qcpy_run_tests() {
  if (!run_test.number_of_suites) {
    printf("No tests to run!\n");
  }

  SRunner *suite_runner = NULL;
  while (run_test.suites) {
    suite_runner = srunner_create(run_test.suites->test_suite);
    run_test.number_of_failures += srunner_ntests_failed(suite_runner);
    run_test.suites = run_test.suites->next_suite;
  }

  srunner_free(suite_runner);
  assert(!suite_runner);

  return run_test.number_of_failures;
}
