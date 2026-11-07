add_library(qcpy_core_boot qcpy_core_boot.c)

add_subdirectory(bootargs)

target_link_libraries(qcpy_core_boot PUBLIC
  qcpy_error
  port
  qlog
  qcpy_core_bootargs
)

target_include_directories(qcpy_core_boot PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
