
add_library(libappimageinfo SHARED
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
        ${PROJECT_SOURCE_DIR}/include/appimage/appimageinfo.h
        lib/appimageinfo.cpp)


target_compile_options(libappimageinfo PUBLIC -fPIC -fexceptions)
target_include_directories(libappimageinfo
        PUBLIC
        ${PROJECT_SOURCE_DIR}/include
        PRIVATE
        ${LIBAPPIMAGE_INCLUDE_DIRS}
        ${LIBBFD_INCLUDE_DIRS}
        ${LIBXML2_INCLUDE_DIR}
        ${OPENSSL_INCLUDE_DIRS}
        ${NLOHMANN_JSON_INCLUDE_DIRS})

target_link_libraries(libappimageinfo
        ${LIBAPPIMAGE_LIBRARIES}
        ${LIBBFD_LIBRARIES}
        ${OPENSSL_LIBRARIES}
        ${LIBXML2_LIBRARIES}
        ${Boost_LIBRARIES}
        )
target_compile_definitions(libappimageinfo PRIVATE ${LIBXML2_DEFINITIONS})

install(TARGETS libappimageinfo
        LIBRARY
        DESTINATION lib
        COMPONENT Libraries
        PUBLIC_HEADER
        DESTINATION include
        COMPONENT Development)

set_target_properties(libappimageinfo PROPERTIES
        VERSION ${LIBAPPIMAGE-INFO_VERSION} SOVERSION ${LIBAPPIMAGE-INFO_VERSION_MAJOR}
        PUBLIC_HEADER ${PROJECT_SOURCE_DIR}/include/appimage/appimageinfo.h
        PREFIX "" # Prevent CMake from adding the 'lib' prefix to the library.
        INSTALL_RPATH_USE_LINK_PATH TRUE
        INSTALL_RPATH "\$ORIGIN/../lib:${CMAKE_INSTALL_PREFIX}/lib")


add_dependencies(libappimageinfo libappimage libappimageinfo)

add_executable(appimageinfotool app/main.cpp)
target_link_libraries(appimageinfotool libappimageinfo ${APPIMAGEKIT_LIBRARIES} ${LIBBFD_LIBRARIES})
target_include_directories(appimageinfotool PRIVATE ${NLOHMANN_JSON_INCLUDE_DIRS})
target_compile_definitions(appimageinfotool PRIVATE PROJECT_NAME="${PROJECT_NAME}" PRIVATE PROJECT_VERSION="${PROJECT_VERSION}")

set_target_properties(appimageinfotool PROPERTIES
        INSTALL_RPATH_USE_LINK_PATH TRUE
        INSTALL_RPATH "\$ORIGIN/../lib:${CMAKE_INSTALL_PREFIX}/lib")

install(TARGETS appimageinfotool DESTINATION bin COMPONENT binaries)
install(FILES app/res/appimageinfotool.desktop DESTINATION share/applications/ COMPONENT desktop)
install(FILES app/res/appimageinfotool.svg DESTINATION share/icons/hicolor/scalable/apps/ COMPONENT desktop)
add_dependencies(appimageinfotool libappimageinfo)
