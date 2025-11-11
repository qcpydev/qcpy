add_library(block block.c)

target_link_libraries(block PUBLIC
   qcpy_error
   port
)

target_include_directories(block PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

