add_library(replay_qlog_register replay_qlog_register.c)

target_include_directories(replay_qlog_register PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(replay_qlog_register PUBLIC
    qlog
    qlog_register
    qcpy_error
)
