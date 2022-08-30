set(LIBRARY_NAME libopenscope)

add_library(${LIBRARY_NAME} STATIC event.cpp openocd.cpp fs.cpp platform.cpp)
target_compile_definitions(${LIBRARY_NAME} PUBLIC FTS_FUZZY_MATCH_IMPLEMENTATION)
target_include_directories(${LIBRARY_NAME} PUBLIC inc)
target_link_libraries(${LIBRARY_NAME} PUBLIC Boost::boost Boost::filesystem)

if (MSVC)
    target_precompile_headers(${LIBRARY_NAME} PUBLIC inc/lib/openocd.h)
endif ()