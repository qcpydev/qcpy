add_library(qlog_node qlog_node.c)

target_link_libraries(qlog_node PRIVATE
    qcpy_error
    qlog_entry
)

target_include_directories(qlog_node PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

