find_package(check REQUIRED)

set(QLOG_TEST_SOURCES
  test_qlog_init.c
  test_qlog_append.c
  test_qlog_clear.c
  test_qlog_combine.c
  test_qlog_compare.c
  test_qlog_delete.c
)

add_executable(run_qlog_tests test_qlog_main.c ${QLOG_TEST_SOURCES})

include_directories(${CHECK_INCLUDE_DIRS})

link_directories(${CHECK_LIBRARY_DIRS})

target_link_libraries(run_qlog_tests PRIVATE
  qlog
  Check::check
)

add_test(NAME qlog_tests COMMAND run_qlog_tests)
