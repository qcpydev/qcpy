add_library(quack_export quack_export.cc)

target_link_libraries(quack_export PRIVATE
    qcpy_error
)

target_include_directories(quack_export PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

