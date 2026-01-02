add_library(qlog_graph_stats qlog_graph_stats.c)

target_link_libraries(qlog_graph_stats PRIVATE
    qcpy_error
)

target_include_directories(qlog_graph_stats PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

