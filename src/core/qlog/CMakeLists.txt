add_library(qlog qlog.c)

add_subdirectory(entry)
add_subdirectory(infra)
add_subdirectory(register)

target_link_libraries(qlog PUBLIC
   qlog_entry
   qcpy_error
   replay_qlog
   qlog_register
   qlog_infra
)

target_include_directories(qlog PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
