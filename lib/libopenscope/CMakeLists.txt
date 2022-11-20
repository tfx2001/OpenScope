cmake_minimum_required(VERSION 3.16)

project(libopenscope)

set(CMAKE_CXX_STANDARD 17)

# For Cargo build
add_subdirectory(cmake/corrosion)
include(cmake/corrosion_cxx.cmake)

find_package(fmt CONFIG REQUIRED)

if (UNIX)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
endif ()

add_library(${PROJECT_NAME} STATIC src/event.cpp src/probe.cpp src/fs.cpp)
add_library_rust(PATH ${CMAKE_CURRENT_SOURCE_DIR}/src/probe_rust NAMESPACE openscope)

target_compile_definitions(${PROJECT_NAME} PUBLIC FTS_FUZZY_MATCH_IMPLEMENTATION)
target_include_directories(${PROJECT_NAME} PUBLIC inc)
target_link_libraries(${PROJECT_NAME} PUBLIC fmt::fmt openscope::probe_rust)

set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_NAME openscope)

if (WIN32)
    target_sources(${PROJECT_NAME} PRIVATE src/platform_windows.cpp)
    target_compile_options(${PROJECT_NAME} PUBLIC -municode)
    target_link_libraries(${PROJECT_NAME} PUBLIC ws2_32)
elseif (UNIX)
    target_sources(${PROJECT_NAME} PRIVATE src/platform_linux.cpp)
    target_compile_options(${PROJECT_NAME} PRIVATE ${GTK3_CFLAGS_OTHER})
    target_include_directories(${PROJECT_NAME} PRIVATE ${GTK3_INCLUDE_DIRS})
    target_link_directories(${PROJECT_NAME} PRIVATE ${GTK3_LIBRARY_DIRS})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${GTK3_LIBRARIES})
endif ()
