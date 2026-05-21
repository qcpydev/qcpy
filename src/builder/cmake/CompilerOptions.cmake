# CLANG FORMAT
# ========================================
file(GLOB_RECURSE CLANG_FORMAT_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cc"
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

# ── Base flags inherited by all custom types ───────────────────────────────────
set(CMAKE_C_FLAGS_DEBUG   "-g -O0" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0" CACHE STRING "" FORCE)

# ── ASan ──────────────────────────────────────────────────────────────────────
set(ASAN_FLAGS "-g -O1 -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls")

set(CMAKE_C_FLAGS_ASAN   "${ASAN_FLAGS}" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS_ASAN "${ASAN_FLAGS}" CACHE STRING "" FORCE)
set(CMAKE_EXE_LINKER_FLAGS_ASAN   "-fsanitize=address" CACHE STRING "" FORCE)
set(CMAKE_SHARED_LINKER_FLAGS_ASAN "-fsanitize=address" CACHE STRING "" FORCE)
mark_as_advanced(CMAKE_C_FLAGS_ASAN CMAKE_CXX_FLAGS_ASAN)

# ── MSan (clang only) ─────────────────────────────────────────────────────────
set(MSAN_FLAGS "-g -O1 -fsanitize=memory -fno-omit-frame-pointer -fsanitize-memory-track-origins")

set(CMAKE_C_FLAGS_MSAN   "${MSAN_FLAGS}" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS_MSAN "${MSAN_FLAGS}" CACHE STRING "" FORCE)
set(CMAKE_EXE_LINKER_FLAGS_MSAN   "-fsanitize=memory" CACHE STRING "" FORCE)
set(CMAKE_SHARED_LINKER_FLAGS_MSAN "-fsanitize=memory" CACHE STRING "" FORCE)
mark_as_advanced(CMAKE_C_FLAGS_MSAN CMAKE_CXX_FLAGS_MSAN)

# ── UBSan ─────────────────────────────────────────────────────────────────────
set(UBSAN_FLAGS "-g -O1 -fsanitize=undefined -fno-omit-frame-pointer")

set(CMAKE_C_FLAGS_UBSAN   "${UBSAN_FLAGS}" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS_UBSAN "${UBSAN_FLAGS}" CACHE STRING "" FORCE)
set(CMAKE_EXE_LINKER_FLAGS_UBSAN   "-fsanitize=undefined" CACHE STRING "" FORCE)
set(CMAKE_SHARED_LINKER_FLAGS_UBSAN "-fsanitize=undefined" CACHE STRING "" FORCE)
mark_as_advanced(CMAKE_C_FLAGS_UBSAN CMAKE_CXX_FLAGS_UBSAN)

# ── TSan ──────────────────────────────────────────────────────────────────────
set(TSAN_FLAGS "-g -O1 -fsanitize=thread")

set(CMAKE_C_FLAGS_TSAN   "${TSAN_FLAGS}" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS_TSAN "${TSAN_FLAGS}" CACHE STRING "" FORCE)
set(CMAKE_EXE_LINKER_FLAGS_TSAN   "-fsanitize=thread" CACHE STRING "" FORCE)
set(CMAKE_SHARED_LINKER_FLAGS_TSAN "-fsanitize=thread" CACHE STRING "" FORCE)
mark_as_advanced(CMAKE_C_FLAGS_TSAN CMAKE_CXX_FLAGS_TSAN)

# ── Tell CMake these are valid build types (for IDEs + validation) ─────────────
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY
    STRINGS "Debug;Release;RelWithDebInfo;MinSizeRel;Asan;Msan;Ubsan;Tsan"
)
