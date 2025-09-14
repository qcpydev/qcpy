add_library(replay_qlog_entry replay_qlog_entry.c)

target_include_directories(replay_qlog_entry PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(replay_qlog_entry PUBLIC
    qlog_entry
    qcpy_error
)
