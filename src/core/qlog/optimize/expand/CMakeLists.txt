add_library(qlog_expand qlog_expand.c)

target_link_libraries(qlog_expand PUBLIC
    qcpy_error
    qlog_node
    qlog_graph
    qcpy_base
)

target_include_directories(qlog_expand PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

