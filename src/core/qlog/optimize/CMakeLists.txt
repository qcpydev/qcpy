add_library(qlog_optimize qlog_optimize.c)

add_subdirectory(clean)
add_subdirectory(expand)
add_subdirectory(stats)

target_link_libraries(qlog_optimize PRIVATE
  block
  qcpy_error
  qlog_entry
  qlog_clean
  qlog_expand
  qlog_optimize_stats
  qcpy_base
)

target_include_directories(qlog_optimize PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
