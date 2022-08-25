set(LIBRARY_NAME widget)

add_library(${LIBRARY_NAME} OBJECT
        widget.cpp sidebar.cpp filter_combo.cpp popup.cpp)
target_include_directories(${LIBRARY_NAME}
        PUBLIC inc)
target_link_libraries(${LIBRARY_NAME} PUBLIC imgui::imgui libopenscope)