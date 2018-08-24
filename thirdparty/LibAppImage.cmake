find_package(libappimage QUIET)

if (NOT libappimage_FOUND)
    message(STATUS "Downloading and building libappimage")
    include(ExternalProject)

    ExternalProject_Add(libappimage
            GIT_REPOSITORY https://github.com/AppImage/libappimage.git
            GIT_TAG master
            GIT_SUBMODULES ""
            CONFIGURE_COMMAND ${CMAKE_COMMAND} -G${CMAKE_GENERATOR} -DBUILD_TESTING=OFF -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} <SOURCE_DIR>
            BUILD_COMMAND make
            INSTALL_COMMAND make install DESTDIR=<INSTALL_DIR>
            )

    ExternalProject_Get_Property(libappimage INSTALL_DIR BINARY_DIR)
    set(LIBAPPIMAGE_INSTALL_DIR ${INSTALL_DIR})

    set(LIBAPPIMAGE_LIBRARIES ${LIBAPPIMAGE_INSTALL_DIR}${CMAKE_INSTALL_PREFIX}/lib/libappimage.so PARENT_SCOPE)
    set(LIBAPPIMAGE_INCLUDE_DIRS ${LIBAPPIMAGE_INSTALL_DIR}${CMAKE_INSTALL_PREFIX}/include/ PARENT_SCOPE)

    install(FILES ${LIBAPPIMAGE_LIBRARIES} DESTINATION lib COMPONENT desktop)
endif (NOT libappimage_FOUND)