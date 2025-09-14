add_library(qcpy_error qcpy_error.c)

target_include_directories(qcpy_error PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(replay/replay_qlog)
add_subdirectory(replay/replay_qlog_entry)
add_subdirectory(replay/replay_qlog_register)

target_link_libraries(qcpy_error PUBLIC
    replay_qlog
    replay_qlog_entry
    qlog
    replay_qlog_register
    qlog_register
)

