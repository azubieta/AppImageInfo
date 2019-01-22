if(NOT TARGET Boost)
    message(STATUS "Downloading and building boost")

    if("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "i386")
        set(BOOST_B2_TARGET_CONFIG architecture=x86 address-model=32)
    endif()

    externalproject_add(Boost_EXTERNAL
        URL https://dl.bintray.com/boostorg/release/1.69.0/source/boost_1_69_0.tar.gz
        URL_HASH SHA256=9a2c2819310839ea373f42d69e733c339b4e9a19deab6bfec448281554aa4dbb
        CONFIGURE_COMMAND ./bootstrap.sh --with-libraries=filesystem,system,thread
        BUILD_COMMAND ./b2 ${BOOST_B2_TARGET_CONFIG} cxxflags=-fPIC ${CPPFLAGS} cflags=-fPIC ${CFLAGS} link=static
        INSTALL_COMMAND ""
        BUILD_IN_SOURCE 1
        )


    add_library(Boost STATIC IMPORTED)

    externalproject_get_property(Boost_EXTERNAL BINARY_DIR)
    set_target_properties(
        Boost PROPERTIES
        INTERFACE_IMPORTED_LOCATION "${BINARY_DIR}/stage/lib/libboost_filesystem.a;${BINARY_DIR}/stage/lib/libboost_system.a;"
        INTERFACE_INCLUDE_DIRECTORIES ${BINARY_DIR}
    )
        message("Boost BINARY_DIR ${BINARY_DIR}")

    add_dependencies(Boost Boost_EXTERNAL)
endif()

