#include "test_qlog_main.h"
#include <check.h>
#include <stdlib.h>

int main() {
  int failed = 0;
  SRunner *runner = srunner_create(NULL);
  srunner_add_suite(runner, qlog_append_suite());
  srunner_add_suite(runner, qlog_init_suite());

  srunner_run_all(runner, CK_VERBOSE);
  failed = srunner_ntests_failed(runner);
  srunner_free(runner);
  return (failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
