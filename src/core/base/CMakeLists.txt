add_library(qcpy_base base.c)

target_include_directories(qcpy_base PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

