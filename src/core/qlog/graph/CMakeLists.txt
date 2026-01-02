add_library(qlog_graph qlog_graph.c)

add_subdirectory(node)
add_subdirectory(stats)

target_link_libraries(qlog_graph PUBLIC
   qlog_entry
   qlog_node
   qlog_graph_stats
   qcpy_error
)

target_include_directories(qlog_graph PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
