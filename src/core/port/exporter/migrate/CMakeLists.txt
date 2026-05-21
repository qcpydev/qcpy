add_library(exporter_migrate exporter_migrate.c)

target_include_directories(exporter_migrate PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(exporter_migrate PUBLIC
    qlog
    importer
    qcpy_error
)
