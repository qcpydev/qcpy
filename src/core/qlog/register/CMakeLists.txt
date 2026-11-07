add_library(qlog_register qlog_register.c)

target_link_libraries(qlog_register PUBLIC
    qcpy_error
    qlog
)

target_include_directories(qlog_register PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

