add_library(error_boot error_boot.c)

target_include_directories(error_importer PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(error_importer PUBLIC
    importer
)
