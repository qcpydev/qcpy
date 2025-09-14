add_library(qlog_thread_pool qlog_thread_pool.c)

target_include_directories(qlog_thread_pool PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(qlog_thread_pool PUBLIC
    qlog
    qlog_entry
    qlog_register
    qcpy_error
)
