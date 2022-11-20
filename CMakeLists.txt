cmake_minimum_required(VERSION 3.16)

cmake_policy(SET CMP0087 NEW)

if (CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR)
    message(FATAL_ERROR "Source and build directories cannot be the same.")
endif ()

if (NOT DEFINED ENV{VCPKG_ROOT})
    message(FATAL_ERROR "Please set environment variable VCPKG_ROOT")
else ()
    message(STATUS "vcpkg path: $ENV{VCPKG_ROOT}")
endif ()

set(CMAKE_TOOLCHAIN_FILE $ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)

project(openscope)

include(cmake/build_helper.cmake)
include(cmake/install.cmake)

find_package(imgui CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(Iconv REQUIRED)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(lib/libopenscope)
add_subdirectory(src)
add_subdirectory(test)
