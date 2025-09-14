add_library(qlog_tests
  test_qlog_init.c
  test_qlog_append.c
  test_qlog_clear.c
  test_qlog_combine.c
  test_qlog_compare.c
  test_qlog_delete.c
)

target_link_libraries(qlog_tests PUBLIC
    test_common
    test_qlog
)

target_link_libraries(qlog_tests PRIVATE
    Check::check qlog
)

add_test(NAME qlog_tests COMMAND run_qlog_tests)

target_include_directories(qlog_tests PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

