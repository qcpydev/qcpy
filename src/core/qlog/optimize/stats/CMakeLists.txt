add_library(qlog_optimize_stats qlog_optimize_stats.c)

target_link_libraries(qlog_optimize_stats PUBLIC
    qcpy_error
)

target_include_directories(qlog_optimize_stats PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

