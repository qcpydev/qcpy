add_library(dock dock.c)

add_subdirectory(logger)

target_link_libraries(dock PUBLIC
   qcpy_error
   qcpy_base
   boot
   base_tools
   dock_logger
   importer
)

target_include_directories(dock PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
