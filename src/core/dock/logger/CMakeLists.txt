add_library(dock_logger dock_logger.c)

target_link_libraries(dock_logger PRIVATE
  block
)

target_include_directories(dock_logger PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
