add_library(qlog_print qlog_print.c)

target_link_libraries(qlog_print PRIVATE
    qcpy_base
    qcpy_error
)

target_include_directories(qlog_print PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

