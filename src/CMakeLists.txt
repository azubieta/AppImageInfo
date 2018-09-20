# configure a header file to pass some of the CMake settings to the source code
configure_file ( "config.h.in" "${PROJECT_BINARY_DIR}/include/appimage/info-config.h")
include_directories("${PROJECT_BINARY_DIR}/include")
include_directories("${PROJECT_SOURCE_DIR}/include")

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
        ${PROJECT_SOURCE_DIR}/include/appimage/info.h
        lib/info.cpp)

target_compile_options(libappimageinfo PUBLIC -fPIC -fexceptions)
target_include_directories(libappimageinfo
        PUBLIC ${APPIMAGE_INCLUDE_DIRS}
        PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        PUBLIC $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
        INTERFACE $<INSTALL_INTERFACE:include/>
        PRIVATE
        ${LIBBFD_INCLUDE_DIRS}
        ${LIBXML2_INCLUDE_DIR}
        ${OPENSSL_INCLUDE_DIRS}
        ${NLOHMANN_JSON_INCLUDE_DIRS})

target_link_libraries(libappimageinfo
        PUBLIC libappimage
        PRIVATE
        ${LIBBFD_LIBRARIES}
        ${OPENSSL_LIBRARIES}
        ${LIBXML2_LIBRARIES}
        ${Boost_LIBRARIES}
        )
target_compile_definitions(libappimageinfo PRIVATE ${LIBXML2_DEFINITIONS})

set_target_properties(libappimageinfo PROPERTIES
        VERSION  ${AppImageInfo_VERSION} SOVERSION ${AppImageInfo_VERSION_MAJOR}
        PUBLIC_HEADER "${PROJECT_SOURCE_DIR}/include/appimage/info.h;${PROJECT_BINARY_DIR}/info-config.h"
        PREFIX "" # Prevent CMake from adding the 'lib' prefix to the library.
        INSTALL_RPATH_USE_LINK_PATH TRUE
        INSTALL_RPATH "\$ORIGIN/../lib:${CMAKE_INSTALL_PREFIX}/lib")


add_dependencies(libappimageinfo libappimage libappimageinfo)

add_executable(appimageinfotool app/main.cpp config.h.in)
target_link_libraries(appimageinfotool libappimage libappimageinfo ${APPIMAGEKIT_LIBRARIES} ${LIBBFD_LIBRARIES})
target_include_directories(appimageinfotool PRIVATE ${NLOHMANN_JSON_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/include)
target_compile_definitions(appimageinfotool PRIVATE PROJECT_NAME="${PROJECT_NAME}" PRIVATE PROJECT_VERSION="${PROJECT_VERSION}")

set_target_properties(appimageinfotool PROPERTIES
        VERSION  ${AppImageInfo_VERSION} SOVERSION ${AppImageInfo_VERSION_MAJOR}
        INSTALL_RPATH_USE_LINK_PATH TRUE
        INSTALL_RPATH "\$ORIGIN/../lib:${CMAKE_INSTALL_PREFIX}/lib")

install(TARGETS libappimageinfo appimageinfotool
        EXPORT AppImageInfoTargets
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin
        LIBRARY DESTINATION "${INSTALL_LIB_DIR}" COMPONENT lib
        PUBLIC_HEADER DESTINATION "${INSTALL_INCLUDE_DIR}/appimage" COMPONENT dev)


install(FILES app/res/appimageinfotool.desktop DESTINATION share/applications/ COMPONENT desktop)
install(FILES app/res/appimageinfotool.svg DESTINATION share/icons/hicolor/scalable/apps/ COMPONENT desktop)
add_dependencies(appimageinfotool libappimageinfo)
