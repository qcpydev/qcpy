add_executable(qcpy_core qcpy_core.c)

target_link_libraries(qcpy_core PRIVATE
  qcpy_base
  qcpy_error
  block
  port
  dock
  boot
  qlog_register
  qlog
)
