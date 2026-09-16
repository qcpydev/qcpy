add_library(qcpy_connect SHARED qcpy_connect.c)

target_link_libraries(qcpy_connect PRIVATE
  qcpy_error
  qcpy_base
  block
  boot
  port
  dock
)
