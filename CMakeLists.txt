cmake_minimum_required(VERSION 3.6)
project(Game-of-Seyeds)

# ensure C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# export compiler flags for code completion engines
set( CMAKE_EXPORT_COMPILE_COMMANDS 1 )

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# Disable all the warnings before building the dependencies
if(MSVC)
    add_compile_options(/w)
else()
    add_compile_options(-Wno-everything)
endif()

# Source: https://github.com/bolu-atx/cmake-curl-example
# Prefer static libs over shared lib
LIST(REVERSE CMAKE_FIND_LIBRARY_SUFFIXES)
##========== Add dependency =================
OPTION(ENABLE_OPENSSL_FROM_SOURCE "BUILD OPENSSL FROM SOURCE AT CONFIGURE TIME" ON)

if(ENABLE_OPENSSL_FROM_SOURCE)
    set(BUILD_OPENSSL_WORKING_DIR ${CMAKE_BINARY_DIR}/_external/build-OpenSSL)
    set(BUILD_OPENSSL_SRC_DIR ${CMAKE_SOURCE_DIR}/CMake/build-OpenSSL)
    set(BUILD_OPENSSL_INSTALL_DIR "${BUILD_OPENSSL_WORKING_DIR}/install-OpenSSL")
    file(MAKE_DIRECTORY ${BUILD_OPENSSL_WORKING_DIR})
    if(NOT EXISTS ${BUILD_OPENSSL_INSTALL_DIR})
        message(STATUS "Building OpenSSL.. at ${BUILD_OPENSSL_WORKING_DIR}, Install DIR ${BUILD_OPENSSL_INSTALL_DIR}")
        execute_process(
                COMMAND ${CMAKE_COMMAND} ${BUILD_OPENSSL_SRC_DIR} -DINSTALL_DIR=${BUILD_OPENSSL_INSTALL_DIR}
                WORKING_DIRECTORY ${BUILD_OPENSSL_WORKING_DIR}
        )
        execute_process(
                COMMAND ${CMAKE_COMMAND} --build .
                WORKING_DIRECTORY ${BUILD_OPENSSL_WORKING_DIR}
        )
    else()
        message(STATUS "Found pre-built openSSL at ${BUILD_OPENSSL_INSTALL_DIR}, skipping rebuild")
    endif()

    set(OPENSSL_INCLUDE_DIR ${BUILD_OPENSSL_INSTALL_DIR}/include)
    add_library(openssl INTERFACE IMPORTED)
    target_include_directories(openssl INTERFACE ${OPENSSL_INCLUDE_DIR})
    target_link_directories(openssl INTERFACE ${BUILD_OPENSSL_INSTALL_DIR}/lib)

	set(OPENSSL_ROOT_DIR "${BUILD_OPENSSL_INSTALL_DIR}" CACHE INTERNAL "OPENSSL ROOT DIR")
	message(MESSAGE "Setting OpenSSL ROOT DIR to newly built OpenSSL ${OPENSSL_ROOT_DIR}")
else()
    find_package(OpenSSL REQUIRED)
endif()

if (MSVC)
	set(CURL_PLATFORM_OPTIONS "CMAKE_USE_WINSSL ON;CMAKE_USE_OPENSSL OFF")
else()
	set(CURL_PLATFORM_OPTIONS "CMAKE_USE_OPENSSL ON")
endif()

#========== Add CPM package manager ================= 
include(./CMake/CPM.cmake)

# Get curl
CPMAddPackage(
		NAME curl
		VERSION 7.67.0
		URL https://github.com/curl/curl/releases/download/curl-7_67_0/curl-7.67.0.tar.gz
        DOWNLOAD_EXTRACT_TIMESTAMP true
		OPTIONS
		"CURL_STATICLIB ON"
		"BUILD_CURL_EXE OFF"
		"BUILD_SHARED_LIBS OFF"
		"BUILD_TESTING OFF"
		"WITH_STATIC_RT ON"
		"CMAKE_USE_LIBSSH2 OFF"
		${CURL_PLATFORM_OPTIONS}
)

# Enable all the warnings for the main project's source code
if(MSVC)
    add_compile_options(/W3 /WX)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

file(GLOB UTILS "src/utils/*.cpp")
set(SOURCE_FILES src/main.cpp ${UTILS})

add_executable(main ${SOURCE_FILES})
target_include_directories(main PUBLIC src)

target_include_directories(main PRIVATE "${curl_SOURCE_DIR}/include")

target_link_libraries(main libcurl)

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
