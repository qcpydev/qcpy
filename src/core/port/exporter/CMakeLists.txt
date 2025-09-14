add_library(exporter exporter.c)

target_include_directories(exporter PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(exporter PUBLIC
    qlog
    qlog_register
    qcpy_error
)
