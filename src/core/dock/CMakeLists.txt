add_library(dock dock.c)

target_link_libraries(dock PUBLIC
   qcpy_error
   qcpy_base
   args
)

target_include_directories(dock PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
