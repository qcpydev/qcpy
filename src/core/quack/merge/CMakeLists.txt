add_library(quack_merge quack_merge.cc)

target_link_libraries(quack_merge PRIVATE
    qcpy_error 
)

target_include_directories(quack_merge PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)


