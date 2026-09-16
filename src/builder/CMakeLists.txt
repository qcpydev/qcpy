# GLOBAL
# ========================================
cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

project(qcpy VERSION 0.2 LANGUAGES C CXX)

if ("${CMAKE_BINARY_DIR}" STREQUAL "${CMAKE_SOURCE_DIR}")
    message(FATAL_ERROR "In-source builds are not allowed.")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)


set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# C STANDARD
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# C++ STANDARD
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(ENABLE_CUDA "CUDA support is enabled" ON)

if (ENABLE_CUDA)
  # Check if CUDA is enabled
  include(CheckLanguage)
  check_language(CUDA)

  if (CMAKE_CUDA_COMPILER)
    enable_language(CUDA)
    message("Enabling CUDA, version: ${CMAKE_CUDA_COMPILER}")

    # CUDA STANDARD
    set(CMAKE_CUDA_STANDARD 20)
    set(CMAKE_CUDA_STANDARD_REQUIRED ON)
  else()
    message("CUDA not found, disabling...")
    set(ENABLE_CUDA OFF)
  endif()

endif()

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/CompilerOptions.cmake)

# END GLOBAL
# ========================================
#

find_package(Check REQUIRED)
enable_testing()
add_subdirectory(test_build)

# Configure all builds
# ========================================

add_subdirectory(../core core)
add_subdirectory(libqcpy_connect_build)
add_subdirectory(core_build)

# END Configure all builds
# ========================================
