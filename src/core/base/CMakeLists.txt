add_library(qcpy_base base.c)

add_subdirectory(tools)

target_link_libraries(qcpy_base PUBLIC
  base_tools
)

target_include_directories(qcpy_base PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

