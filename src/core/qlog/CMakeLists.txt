add_library(qlog qlog.c)

add_subdirectory(entry)
add_subdirectory(infra)
add_subdirectory(graph)
add_subdirectory(optimize)

target_link_libraries(qlog PUBLIC
   block
   qlog_entry
   qlog_graph
   qlog_optimize
   qcpy_error
)

target_include_directories(qlog PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
