#
# qcpy testing
#

add_subdirectory(test_common)
add_subdirectory(test_qlog)

add_executable(run_tests
  run_tests.c
)

add_test(NAME run_tests COMMAND run_tests)
