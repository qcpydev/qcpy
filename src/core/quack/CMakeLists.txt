add_library(quack quack.cc)

add_subdirectory(dense)
add_subdirectory(export)
add_subdirectory(import)
add_subdirectory(gpu)
add_subdirectory(hive)
add_subdirectory(merge)
add_subdirectory(start)
add_subdirectory(tetris)

target_link_libraries(quack PUBLIC
    quack_dense
    quack_export
    quack_import
    quack_hive
    quack_merge
    quack_start
    quack_tetris
)

if (CMAKE_CUDA_COMPILER)
    target_link_libraries(quack PUBLIC
        quack_gpu
    )
endif()

target_include_directories(quack PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
