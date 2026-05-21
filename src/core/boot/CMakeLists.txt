add_library(boot boot.c)

add_subdirectory(args)

target_link_libraries(boot PUBLIC
   qcpy_error
   boot_args
   port
   qlog_infra
)

target_include_directories(boot PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

