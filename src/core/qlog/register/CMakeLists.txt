add_library(qlog_register qlog_register.c)

target_link_libraries(qlog_register PRIVATE
    qlog
    qcpy_error
)

target_include_directories(qlog_register PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

