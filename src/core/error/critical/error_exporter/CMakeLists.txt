add_library(error_exporter error_exporter.c)

target_include_directories(error_exporter PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(error_exporter PUBLIC
    exporter
)
