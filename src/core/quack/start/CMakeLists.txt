add_library(quack_start quack_start.cc)

target_link_libraries(quack_start PRIVATE
    qcpy_error 
)

target_include_directories(quack_start PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)


