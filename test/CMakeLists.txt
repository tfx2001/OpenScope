enable_testing()

file(GLOB_RECURSE SOURCES *.cpp)

foreach (FILE IN LISTS SOURCES)
    get_filename_component(FILE ${FILE} NAME_WE)
    add_executable(${FILE} ${FILE}.cpp)
    target_link_libraries(${FILE} PRIVATE libopenscope fmt::fmt)
    set_target_properties(${FILE} PROPERTIES
                          RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    add_test(NAME ${FILE}
            COMMAND $<TARGET_FILE:${FILE}>)
endforeach ()