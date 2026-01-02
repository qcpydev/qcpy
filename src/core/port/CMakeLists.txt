add_library(port STATIC port.c)

add_subdirectory(importer)
add_subdirectory(exporter)

target_link_libraries(port PUBLIC
   block
   qcpy_error
   importer
   exporter
)

target_include_directories(port PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
