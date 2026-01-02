add_library(qlog_infra qlog_infra.c)

add_subdirectory(qlog_scheduler)
add_subdirectory(qlog_thread_pool)

target_link_libraries(qlog_infra PUBLIC
   qcpy_error
   qlog_register
   qlog_scheduler
   qlog_thread_pool
   qlog
   importer
)

target_include_directories(qlog_infra PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
