add_library(qcpy_error qcpy_error.c)

target_include_directories(qcpy_error PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(replay/replay_qlog)
add_subdirectory(replay/replay_qlog_entry)
add_subdirectory(replay/replay_qlog_register)
add_subdirectory(critical/error_importer)

target_link_libraries(qcpy_error PUBLIC
    error_importer
    replay_qlog
    replay_qlog_entry
    qlog
    replay_qlog_register
    qlog_register
)

