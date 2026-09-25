add_library(quack_import quack_import.cc)

target_link_libraries(quack_import PRIVATE
    quack_start
    quack_tetris

  exporter_migrate
    qcpy_error
    base_tools
    block
)

target_include_directories(quack_import PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

