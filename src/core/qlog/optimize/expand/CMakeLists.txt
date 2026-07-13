add_library(qlog_expand qlog_expand.c)

target_link_libraries(qlog_expand PUBLIC
    block
    qcpy_error
    qcpy_base
    qlog_node
    qlog_entry
    qlog_graph
)

target_include_directories(qlog_expand PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

