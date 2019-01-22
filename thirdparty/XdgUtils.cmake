if(NOT TARGET XdgUtils)
    ## XdgUtils
    message(STATUS "Downloading and building XdgUtils")

    externalproject_add(
        XdgUtils_EXTERNAL
        GIT_REPOSITORY https://github.com/azubieta/xdg-utils.git
        GIT_TAG master
        GIT_SHALLOW On
        CMAKE_ARGS
        -DCMAKE_POSITION_INDEPENDENT_CODE=On
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
        -DCMAKE_C_FLAGS=${CMAKE_C_FLAGS}

        INSTALL_COMMAND ""
    )


    add_library(XdgUtils STATIC IMPORTED)

    externalproject_get_property(XdgUtils_EXTERNAL BINARY_DIR)
    externalproject_get_property(XdgUtils_EXTERNAL SOURCE_DIR)

    set_target_properties(
        XdgUtils PROPERTIES
        INTERFACE_IMPORTED_LOCATION "${BINARY_DIR}/src/DesktopEntry/libDesktopEntry.a;${BINARY_DIR}/src/BaseDir/libBaseDir.a;"
        INTERFACE_INCLUDE_DIRECTORIES ${SOURCE_DIR}/include
    )

    add_dependencies(XdgUtils XdgUtils_EXTERNAL)


endif()
