add_library(qlog_entry qlog_entry.c)

add_subdirectory(stats)

target_link_libraries(qlog_entry PRIVATE
    replay_qlog_entry
    qlog_entry_stats
    qcpy_error
)

target_include_directories(qlog_entry PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

