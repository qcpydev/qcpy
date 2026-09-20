add_library(port STATIC port.c)
add_subdirectory(exporter)

target_link_libraries(port PUBLIC
   block
   qcpy_error
   exporter
   qlog
)

target_include_directories(port PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
