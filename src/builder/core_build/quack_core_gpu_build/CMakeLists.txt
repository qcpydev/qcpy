add_executable(quack_core_gpu quack_core_gpu.cu)

target_link_libraries(quack_core_gpu PRIVATE
  qcpy_base
  qcpy_error
  qlog
  quack
)
