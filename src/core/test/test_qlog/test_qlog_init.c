#include <check.h>
#include <qlog.h>
#include <stdio.h>
#include <stdlib.h>
#include <test_common.h>

START_TEST(test_qlog_init_1) {
  int qubits = 3;
  qlog_t *qlog = qlog_init(qubits);
  ck_assert(qlog);
  qlog_delete(qlog);
}
END_TEST

START_TEST(test_qlog_init_2) {
  int qubits = 20;
  qlog_t *qlog = qlog_init(qubits);
  ck_assert(qlog);
  qlog_delete(qlog);
}
END_TEST

START_TEST(test_qlog_init_3) {
  int qubits = 0;
  qlog_t *qlog = qlog_init(qubits);
  ck_assert(!qlog);
}
END_TEST

void qlog_init_suite(void) {
  Suite *suite;
  TCase *tc_core;

  suite = suite_create("qlog init");
  tc_core = tcase_create("core");

  tcase_set_timeout(tc_core, 10);
  tcase_add_test(tc_core, test_qlog_init_1);
  tcase_add_test(tc_core, test_qlog_init_2);
  tcase_add_test(tc_core, test_qlog_init_3);
  suite_add_tcase(suite, tc_core);

  assert(run_tests_add_suite(suite));
}
