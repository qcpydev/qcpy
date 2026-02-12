add_library(quack_hive quack_hive.cc)

target_link_libraries(quack_hive PRIVATE
    qcpy_error
)

target_include_directories(quack_hive PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

