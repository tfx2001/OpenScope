cmake_minimum_required(VERSION 3.11)

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

if(MSVC)
    include(cmake/msvc.cmake)
endif()

find_package(imgui CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(Boost REQUIRED COMPONENTS filesystem)
find_package(elfio CONFIG REQUIRED)
find_package(Iconv REQUIRED)

set(CMAKE_CXX_STANDARD 17)

add_compile_definitions(UNICODE)

add_subdirectory(src)
add_subdirectory(test)
