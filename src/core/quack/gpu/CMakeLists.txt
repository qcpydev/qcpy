if (CMAKE_CUDA_COMPILER)
add_library(quack_gpu quack_gpu.cu)

target_link_libraries(quack_gpu PRIVATE
    qcpy_error
)

target_include_directories(quack_gpu PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
endif()
