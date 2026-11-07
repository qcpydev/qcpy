add_library(qlog qlog.c)

add_subdirectory(entry)
add_subdirectory(infra)
add_subdirectory(graph)
add_subdirectory(optimize)
add_subdirectory(register)
add_subdirectory(print)

target_link_libraries(qlog PUBLIC
   qcpy_base
   qcpy_error
   block
   qlog_entry
   qlog_graph
   qlog_optimize
   exporter
   qlog_register
   qlog_print
)

target_include_directories(qlog PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
