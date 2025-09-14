add_subdirectory(args)

add_library(boot boot.c)

target_link_libraries(boot PUBLIC
   qcpy_error
   args
   port
   qlog_infra
   qlog
)

target_include_directories(boot PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

