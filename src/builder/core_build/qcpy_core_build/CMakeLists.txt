add_executable(qcpy_core qcpy_core.c)

target_link_libraries(qcpy_core PRIVATE
  qcpy_error
  block
  port
  dock
  qcpy_core_boot
  qlog_register
  qlog
  qcpy_base
)
