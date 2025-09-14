add_library(importer importer.c)

target_include_directories(importer PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(importer PUBLIC
    qlog
    qlog_register
    qcpy_error
)
