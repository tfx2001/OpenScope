cmake_minimum_required(VERSION 3.16)

project(libopenscope)

set(CMAKE_CXX_STANDARD 17)

find_package(fmt CONFIG REQUIRED)
find_package(Boost REQUIRED COMPONENTS filesystem)

if (UNIX)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
endif ()

add_library(${PROJECT_NAME} SHARED event.cpp openocd.cpp fs.cpp)

if (WIN32)
    target_sources(${PROJECT_NAME} PRIVATE platform_windows.cpp)
elseif (UNIX)
    target_sources(${PROJECT_NAME} PRIVATE platform_linux.cpp)
    target_compile_options(${PROJECT_NAME} PRIVATE ${GTK3_CFLAGS_OTHER})
    target_include_directories(${PROJECT_NAME} PRIVATE ${GTK3_INCLUDE_DIRS})
    target_link_directories(${PROJECT_NAME} PRIVATE ${GTK3_LIBRARY_DIRS})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${GTK3_LIBRARIES})
endif ()

target_compile_definitions(${PROJECT_NAME} PUBLIC FTS_FUZZY_MATCH_IMPLEMENTATION)
target_include_directories(${PROJECT_NAME} PUBLIC inc)
target_link_libraries(${PROJECT_NAME} PUBLIC Boost::boost Boost::filesystem fmt::fmt)

set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
        OUTPUT_NAME openscope)

if (MINGW)
    target_link_libraries(${PROJECT_NAME} PUBLIC ws2_32)
endif ()
