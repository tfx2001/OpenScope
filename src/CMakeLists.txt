add_executable(
        ${PROJECT_NAME}
        main.cpp
        window/window.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE window/inc)

add_subdirectory(widget)
add_subdirectory(lib)

set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
target_link_libraries(${PROJECT_NAME}
        PRIVATE widget libopenscope
        imgui::imgui fmt::fmt)

if (UNIX)
    target_link_libraries(${PROJECT_NAME} PRIVATE GL)
elseif (MSVC)
    target_link_options(${PROJECT_NAME} PUBLIC /SUBSYSTEM:windows /ENTRY:mainCRTStartup)
    target_link_libraries(${PROJECT_NAME} PRIVATE opengl32)
endif ()