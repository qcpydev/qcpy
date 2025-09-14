add_library(qlog_scheduler qlog_scheduler.c)

target_include_directories(qlog_scheduler PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(qlog_scheduler PUBLIC
    qlog
    qlog_entry
    qlog_register
    qcpy_error
)
