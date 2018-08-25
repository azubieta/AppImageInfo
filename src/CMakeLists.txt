find_package(Bfd REQUIRED)

add_library(libappimage-info SHARED
        entities/FileMetadataExtractor.cpp
        entities/FileMetadataExtractor.h
        entities/DesktopFileMetadataExtractor.cpp
        entities/DesktopFileMetadataExtractor.h
        entities/AppStreamMetadataExtractor.cpp
        entities/AppStreamMetadataExtractor.h
        entities/FileMetadataMerger.cpp
        entities/FileMetadataMerger.h
        entities/BinaryMetadataExtractor.cpp
        entities/BinaryMetadataExtractor.h
        ${PROJECT_SOURCE_DIR}/include/appimage/appimage-info.h
        lib/appimage-info-impl.cpp)


set_target_properties(libappimage-info PROPERTIES PUBLIC_HEADER ${PROJECT_SOURCE_DIR}/include/appimage/appimage-info.h)
target_compile_options(libappimage-info PUBLIC -fPIC -fexceptions)
target_include_directories(libappimage-info
        PUBLIC
        ${PROJECT_SOURCE_DIR}/include
        PRIVATE
        ${LIBAPPIMAGE_INCLUDE_DIRS}
        ${LIBBFD_INCLUDE_DIRS}
        ${LIBXML2_INCLUDE_DIR}
        ${OPENSSL_INCLUDE_DIRS}
        ${NLOHMANN_JSON_INCLUDE_DIRS})

target_link_libraries(libappimage-info
        ${LIBAPPIMAGE_LIBRARIES}
        ${LIBBFD_LIBRARIES}
        ${OPENSSL_LIBRARIES}
        ${LIBXML2_LIBRARIES}
        ${Boost_LIBRARIES}
        )
target_compile_definitions(libappimage-info PRIVATE ${LIBXML2_DEFINITIONS})

set_target_properties(libappimage-info PROPERTIES VERSION ${LIBAPPIMAGE-INFO_VERSION}
        SOVERSION ${LIBAPPIMAGE-INFO_VERSION_MAJOR} )

# Prevent CMake from adding the 'lib' prefix to the library.
set_target_properties(libappimage-info PROPERTIES PREFIX "")

install(TARGETS libappimage-info
        LIBRARY
        DESTINATION lib
        COMPONENT Libraries
        PUBLIC_HEADER
        DESTINATION include
        COMPONENT Development)

set_target_properties(libappimage-info PROPERTIES
        BUILD_WITH_INSTALL_RPATH TRUE
        INSTALL_RPATH_USE_LINK_PATH TRUE
        INSTALL_RPATH "\$ORIGIN/../lib:${CMAKE_INSTALL_PREFIX}/lib")

if (NOT AppImageKit_FOUND)
    add_dependencies(libappimage-info libappimage)
endif ()

add_executable(appimage-info app/main.cpp)
target_link_libraries(appimage-info libappimage-info ${APPIMAGEKIT_LIBRARIES} ${LIBBFD_LIBRARIES})
target_include_directories(appimage-info PRIVATE ${NLOHMANN_JSON_INCLUDE_DIRS})
target_compile_definitions(appimage-info PRIVATE PROJECT_NAME="${PROJECT_NAME}" PRIVATE PROJECT_VERSION="${PROJECT_VERSION}")

set_target_properties(appimage-info PROPERTIES
        BUILD_WITH_INSTALL_RPATH TRUE
        INSTALL_RPATH_USE_LINK_PATH TRUE
        INSTALL_RPATH "\$ORIGIN/../lib:${CMAKE_INSTALL_PREFIX}/lib")

install(TARGETS appimage-info DESTINATION bin COMPONENT binaries)
install(FILES app/res/appimage-info.desktop DESTINATION share/applications/ COMPONENT desktop)
install(FILES app/res/appimage-info.svg DESTINATION share/icons/hicolor/scalable/apps/ COMPONENT desktop)

add_dependencies(appimage-info libappimage-info)
