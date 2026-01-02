add_library(importer STATIC importer.c)

target_include_directories(importer PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(importer PRIVATE
    block
    qlog_infra
    qcpy_error
)
