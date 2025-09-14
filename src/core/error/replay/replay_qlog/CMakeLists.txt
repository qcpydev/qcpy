add_library(replay_qlog replay_qlog.c)

target_include_directories(replay_qlog PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(replay_qlog PUBLIC
    qlog
    qcpy_error
)
