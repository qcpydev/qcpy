add_library(base_tools base_tools.c)

target_link_libraries(base_tools PRIVATE
  block
)

target_include_directories(base_tools PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
