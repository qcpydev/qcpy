add_library(qlog qlog.c)

add_subdirectory(entry)
add_subdirectory(infra)
add_subdirectory(graph)
add_subdirectory(optimize)
add_subdirectory(register)

target_link_libraries(qlog PUBLIC
   qcpy_error
   block
   qlog_entry
   qlog_graph
   qlog_optimize
   qcpy_base
   exporter
   qlog_register
)

target_include_directories(qlog PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
