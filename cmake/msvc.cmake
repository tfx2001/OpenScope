if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	message(STATUS "Add incrimental compile option for MSVC")
	add_compile_options(/ZI)
	add_link_options(/INCRIMENTAL)
endif()