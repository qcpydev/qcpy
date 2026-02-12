add_library(quack_tetris quack_tetris.cc)

target_link_libraries(quack_tetris PRIVATE
    qcpy_error 
)

target_include_directories(quack_tetris PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)


