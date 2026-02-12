add_library(quack_import quack_import.cc)

target_link_libraries(quack_import PRIVATE
    qcpy_error
)

target_include_directories(quack_import PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

