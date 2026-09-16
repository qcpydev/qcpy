add_library(qcpy_error qcpy_error.c)

target_include_directories(qcpy_error PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(replay)

target_link_libraries(qcpy_error PRIVATE
    replay_qlog
    replay_qlog_entry
    replay_qlog_register
    qlog
    qlog_register
)
