add_library(test_common test_common.c)

target_include_directories(test_common PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
