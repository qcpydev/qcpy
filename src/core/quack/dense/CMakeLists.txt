add_library(quack_dense quack_dense.cc)

target_link_libraries(quack_dense PRIVATE
    qcpy_error
)

target_include_directories(quack_dense PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

