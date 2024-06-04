cmake_minimum_required(VERSION 3.6)
project(Game-of-Seyeds)

# ensure C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# export compiler flags for code completion engines
set( CMAKE_EXPORT_COMPILE_COMMANDS 1 )

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# Set global compiler warnings
if(MSVC)
    add_compile_options(/W3 /WX)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

add_executable(main src/main.cpp)
target_include_directories(main PUBLIC src)

# Clang tidy as optional static analyzer
option(Seyed_USE_CLANG_TIDY "Use clang tidy for static analysis" OFF)
if (Seyed_USE_CLANG_TIDY)
    find_program(
        CLANG_TIDY_EXE NAMES "clang-tidy" "clang-tidy-6.0" "clang-tidy-5.0" "clang-tidy-4.0"
        DOC "Path to clang-tidy executable"
    )
    if(NOT CLANG_TIDY_EXE)
      message(STATUS "clang-tidy not found.")
    else()
      message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
      set_target_properties(mylib PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
    endif()
endif()

# LLVM AddressSanitizer for detecting memory errors
# Note that there are many other sanitizers in LLVM to help detect errors, see
# http://travistoptips.blogspot.co.uk/2015/11/sanitize-all-things.html
option(Seyed_MEMCHECK "Use LLVM AddressSanitizer to detecting memory errors" OFF)
if (Seyed_MEMCHECK)
    if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        message(STATUS "Configuring with LLVM AddressSanitizer")
        set(Template_MEMCHECK_FLAGS -fno-optimize-sibling-calls 
                                    -fsanitize=address 
                                    -fsanitize-address-use-after-scope
                                    )
        target_compile_options(mylib PUBLIC -O1 -g -fno-omit-frame-pointer ${Template_MEMCHECK_FLAGS})
        target_link_libraries(mylib PUBLIC -g ${Template_MEMCHECK_FLAGS})
    else()
        message(FATAL_ERROR "clang compiler required with Seyed_MEMCHECK: found ${CMAKE_CXX_COMPILER_ID}")
    endif()
endif()
