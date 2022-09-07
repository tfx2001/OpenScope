if (WIN32)
    install(CODE [[
    file(GET_RUNTIME_DEPENDENCIES
        EXECUTABLES $<TARGET_FILE:openscope>
        RESOLVED_DEPENDENCIES_VAR _r_deps
        UNRESOLVED_DEPENDENCIES_VAR _u_deps
        CONFLICTING_DEPENDENCIES_PREFIX _c_deps
        DIRECTORIES ${DEP_FOLDERS} $ENV{PATH}
        POST_EXCLUDE_REGEXES ".*system32/.*\\.dll"
    )

    if(_u_deps)
        message(WARNING "There were unresolved dependencies for binary: \"${_u_deps}\"!")
    endif()
    if(_c_deps_FILENAMES)
        message(WARNING "There were conflicting dependencies for library: \"${_c_deps}\"!")
    endif()

    foreach(_file ${_r_deps})
        file(INSTALL
            DESTINATION "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}"
            TYPE SHARED_LIBRARY
            FOLLOW_SYMLINK_CHAIN
            FILES "${_file}"
            )
    endforeach()
    ]])
endif ()
