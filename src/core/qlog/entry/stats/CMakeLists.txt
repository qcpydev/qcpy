add_library(qlog_entry_stats qlog_entry_stats.c)

target_include_directories(qlog_entry_stats PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
