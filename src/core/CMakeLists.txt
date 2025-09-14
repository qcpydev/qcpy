cmake_minimum_required(VERSION 3.20)

if ("${CMAKE_BINARY_DIR}" STREQUAL "${CMAKE_SOURCE_DIR}")
    message(FATAL_ERROR "In-source builds are not allowed.")
endif()

project(qcpy-core VERSION 1.0)

set(C_STANDARD 99)
set(C_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/W3)
else()
  add_compile_options(-Wall -Wextra -Werror -shared -pthread -fPIC)
endif()

list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

# CLANG FORMAT
# ========================================
file(GLOB_RECURSE CLANG_FORMAT_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.m"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.mm"
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

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Asan")
endif()

set(CMAKE_PREFIX_PATH "/opt/check" ${CMAKE_PREFIX_PATH})

if(MSVC)
    if(NOT "Asan" IN_LIST CMAKE_CONFIGURATION_TYPES)
        list(APPEND CMAKE_CONFIGURATION_TYPES Asan)
    endif()

    set(CMAKE_EXE_LINKER_FLAGS_ASAN "/DEBUG /INCREMENTAL:NO")
    add_compile_options("$<$<CONFIG:Asan>:/DEBUG /fsanitize=address>")
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


# QLOG CORE EXECUTABLE
# ========================================
add_executable(qcpy_core main.c)

# set up testing
# ========================================
find_package(check REQUIRED)
enable_testing()
add_subdirectory(test)
# ========================================

add_subdirectory(qlog)
add_subdirectory(error)
add_subdirectory(base)
add_subdirectory(boot)
add_subdirectory(port)

target_link_libraries(qcpy_core PRIVATE
    qlog
    qcpy_error
    qcpy_base
    boot
    port
)
# END QLOG CORE EXECUTABLE
# ========================================
