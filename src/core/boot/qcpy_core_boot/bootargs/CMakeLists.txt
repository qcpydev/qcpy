add_library(qcpy_core_bootargs qcpy_core_bootargs.c)

target_link_libraries(qcpy_core_boot PUBLIC
  qcpy_error
  qlog
  qcpy_core_bootargs
)

target_include_directories(qcpy_core_boot PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
