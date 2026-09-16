add_library(boot_args boot_args.c)

target_include_directories(boot_args PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
