add_library(args args.c)

target_include_directories(args PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
