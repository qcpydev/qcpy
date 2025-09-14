#include <check.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct run_suite_s run_suite_t;

struct run_suite_s {
  Suite *test_suite;
  run_suite_t *next_suite;
};

typedef struct run_tests_s {
  run_suite_t *suites;
  run_suite_t *last_suite;
  uint64_t number_of_suites;
  uint64_t number_of_failures;
} run_test_t;

extern run_test_t run_test;

uint64_t qcpy_run_tests();
run_suite_t *run_tests_new_suite(Suite *suite);
bool run_tests_add_suite(Suite *suite);
void clean_suite();
void clean_run_tests();
