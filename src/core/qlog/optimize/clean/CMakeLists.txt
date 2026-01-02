add_library(qlog_clean qlog_clean.c)

target_link_libraries(qlog_clean PUBLIC
    qcpy_error
    qlog_node
    qlog_graph
    qcpy_base
)

target_include_directories(qlog_clean PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

