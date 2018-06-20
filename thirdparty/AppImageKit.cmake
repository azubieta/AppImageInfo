find_package(AppImageKit QUIET)

if (NOT AppImageKit_FOUND)
    message(STATUS "Downloading and building AppImageKit")
    include(ExternalProject)

    ExternalProject_Add(AppImageKit
            GIT_REPOSITORY https://github.com/AppImage/AppImageKit.git
            GIT_TAG appimagetool/master
            GIT_SUBMODULES ""
            CONFIGURE_COMMAND ${CMAKE_COMMAND} -G${CMAKE_GENERATOR} -DBUILD_TESTING=OFF -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} <SOURCE_DIR>
            BUILD_COMMAND make libappimage
            INSTALL_COMMAND make install DESTDIR=<INSTALL_DIR>
            )

    ExternalProject_Get_Property(AppImageKit INSTALL_DIR)
    set(AppImageKit_INSTALL_DIR ${INSTALL_DIR})

    set(APPIMAGEKIT_LIBRARIES ${AppImageKit_INSTALL_DIR}${CMAKE_INSTALL_PREFIX}/lib/libappimage.so)
    set(APPIMAGEKIT_INCLUDE_DIRS ${AppImageKit_INSTALL_DIR}${CMAKE_INSTALL_PREFIX}/include/)

    install(FILES ${APPIMAGEKIT_LIBRARIES} DESTINATION lib COMPONENT desktop)
endif (NOT AppImageKit_FOUND)