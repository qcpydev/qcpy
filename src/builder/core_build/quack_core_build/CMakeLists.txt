add_executable(quack_core quack_core.cc)
target_link_libraries(quack_core PRIVATE
  qcpy_base
  qcpy_error
  qlog
  quack
)
