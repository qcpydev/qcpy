#include <check.h>
#include <qlog.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

START_TEST(test_qlog_append_1) {

  uint8_t qubits = 1;
  qlog_t *qlog = qlog_init(qubits);
  qlog_delete(qlog);
}
END_TEST

Suite *qlog_append_suite(void) {
  Suite *suite;
  TCase *tc_core;

  suite = suite_create("qlog append");
  tc_core = tcase_create("core");

  tcase_set_timeout(tc_core, 10);
  tcase_add_test(tc_core, test_qlog_append_1);
  suite_add_tcase(suite, tc_core);

  return suite;
}

int main() {
  int number_failed;
  Suite *suite;
  SRunner *suite_runner;

  suite = qlog_append_suite();

  suite_runner = srunner_create(suite);

  srunner_run_all(suite_runner, CK_NORMAL);
  number_failed = srunner_ntests_failed(suite_runner);
  srunner_free(suite_runner);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
