add_executable(
        ${PROJECT_NAME}
        main.cpp
        window/window.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE window/inc)

add_subdirectory(widget)
set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
target_link_libraries(${PROJECT_NAME}
        PRIVATE widget
        imgui::imgui fmt::fmt)

if (UNIX)
    target_link_libraries(${PROJECT_NAME} PRIVATE GL)
elseif (MINGW)
    target_sources(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/res/resource.rc)
    target_link_options(${PROJECT_NAME} PRIVATE -mwindows)
    target_link_libraries(${PROJECT_NAME} PRIVATE opengl32)
endif ()