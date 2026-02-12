# GLOBAL
# ========================================

cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

if ("${CMAKE_BINARY_DIR}" STREQUAL "${CMAKE_SOURCE_DIR}")
    message(FATAL_ERROR "In-source builds are not allowed.")
endif()

project(qcpy-core VERSION 0.1 LANGUAGES C CXX)
enable_language(CUDA OPTIONAL)


set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# C STANDARD
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# C++ STANDARD
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# CUDA STANDARD
set(CMAKE_CUDA_STANDARD 20)
set(CMAKE_CUDA_STANDARD_REQUIRED ON)

add_compile_options(-g -O0 -Wall -Wextra -pthread -g3)
list(APPEND CUDA_NVCC_FLAGS "--compiler-options -fPIC")

list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

# END GLOBAL
# ========================================

# CLANG FORMAT
# ========================================
file(GLOB_RECURSE CLANG_FORMAT_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.m"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.mm"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cu"
)

set(CLANG_FORMAT_EXCLUDE_PATTERNS ${CLANG_FORMAT_EXCLUDE_PATTERNS} "/CMakeFiles/" "cmake" "build/")

foreach (SOURCE_FILE ${CLANG_FORMAT_SOURCES})
    foreach (EXCLUDE_PATTERN ${CLANG_FORMAT_EXCLUDE_PATTERNS})
        string(FIND ${SOURCE_FILE} ${EXCLUDE_PATTERN} EXCLUDE_FOUND)
        if (NOT ${EXCLUDE_FOUND} EQUAL -1)
            list(REMOVE_ITEM CLANG_FORMAT_SOURCES ${SOURCE_FILE})
        endif ()
    endforeach ()
endforeach ()

find_program(CLANG_FORMAT_EXECUTABLE NAMES clang-format)

if(CLANG_FORMAT_EXECUTABLE)
    message("Formatting using clang-format")
    add_custom_target(clangformat
        COMMAND ${CLANG_FORMAT_EXECUTABLE} -i --Werror -style=llvm --verbose
        ${CLANG_FORMAT_SOURCES}
        COMMENT "Formatting source files with clang-format"
        VERBATIM
    )
else()
    message(FATAL_ERROR "clang-format not found. Code formatting target will not be available.")
endif()

# END CLANG FORMAT
# ========================================


# ========================================
set(CMAKE_MAP_IMPORTED_CONFIG_DEBUG RELEASE)
set(CMAKE_MAP_IMPORTED_CONFIG_ASAN RELEASE)
set(CMAKE_MAP_IMPORTED_CONFIG_LSAN RELEASE)
set(CMAKE_MAP_IMPORTED_CONFIG_TSAN RELEASE)
set(CMAKE_MAP_IMPORTED_CONFIG_MSAN RELEASE)
set(CMAKE_MAP_IMPORTED_CONFIG_UBSAN RELEASE)

set(CMAKE_PREFIX_PATH "/opt/check" ${CMAKE_PREFIX_PATH})

if(MSVC)
    if(NOT "Asan" IN_LIST CMAKE_CONFIGURATION_TYPES)
        list(APPEND CMAKE_CONFIGURATION_TYPES Asan)
    endif()

    set(CMAKE_EXE_LINKER_FLAGS_ASAN "/DEBUG /INCREMENTAL:NO")
else()
    set(CMAKE_C_FLAGS_ASAN
        "-fsanitize=address -fno-optimize-sibling-calls -fsanitize-address-use-after-scope -fno-omit-frame-pointer -g -O1"
        CACHE STRING "Build with AddressSanitizer"
        FORCE)

    set(CMAKE_C_FLAGS_LSAN
        "-fsanitize=leak -fno-omit-frame-pointer -g -O1"
        CACHE STRING "Build with LeakSanitizer"
        FORCE)

    set(CMAKE_CXX_FLAGS_TSAN
        "-fsanitize=thread -g -O1"
        CACHE STRING "Build with ThreadSanitizer"
        FORCE)

    set(CMAKE_C_FLAGS_MSAN
        "-fsanitize=memory -fno-optimize-sibling-calls -fsanitize-memory-track-origins=2 -fno-omit-frame-pointer -g -O2"
        CACHE STRING "Build with MemorySanitizer"
        FORCE)

    set(CMAKE_C_FLAGS_UBSAN
        "-fsanitize=undefined"
        CACHE STRING "Build with UndefinedBehaviourSanitizer"
        FORCE)
endif()


# QCPY CORE SHARED LIBRARY
# ========================================
add_library(qcpy_connect SHARED qcpy_connect.c)
add_executable(qcpy_core qcpy_core.c)
add_executable(quack_core quack_core.cc)

if (CMAKE_CUDA_COMPILER)
    add_executable(quack_core_gpu quack_core_gpu.cu)
endif()

# QUACK SHARED LIBRARY
# ========================================

# set up testing
# ========================================
#find_package(check REQUIRED)
#enable_testing()
#add_subdirectory(test)
# ========================================

# qcpy_core and qcpy_connect directories
# ========================================
add_subdirectory(qlog)
add_subdirectory(register)
add_subdirectory(error)
add_subdirectory(base)
add_subdirectory(boot)
add_subdirectory(port)
add_subdirectory(dock)
add_subdirectory(block)

# end qcpy_core and qcpy_connect directories
# ========================================

# quack_core and quack_core directories

# QCPY CORE EXECUTABLE
# ========================================

target_link_libraries(qcpy_connect PUBLIC
    qcpy_error
    block
    qcpy_base
    dock
    args
)
target_compile_options(qcpy_connect PUBLIC -Werror)

target_link_libraries(qcpy_core PRIVATE
    qcpy_error
    block
    qlog_register
    qlog
    qcpy_base
    boot
    port
)
target_compile_options(qcpy_core PRIVATE -Werror)

# END QCPY CORE EXECUTABLE
# ========================================

# QUACK DIRECTORIES
# ========================================
add_subdirectory(quack)
# END QUACK DIRECTORIES
# ========================================


# QUACK CORE EXECUTABLE
# ========================================

target_link_libraries(quack_core PRIVATE
    qcpy_error
    quack
)
target_compile_options(qcpy_core PRIVATE -Werror)

# END QUACK CORE EXECUTABLE
# ========================================

# QUACK CORE GPU EXECUTABLE
# ========================================

if (CMAKE_CUDA_COMPILER)
    target_link_libraries(quack_core_gpu PRIVATE
        qcpy_error
        quack
    )
endif()

# END QUACK CORE GPU EXECUTABLE
# ========================================


