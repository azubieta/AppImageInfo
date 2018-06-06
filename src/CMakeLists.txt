find_package(AppImageKit REQUIRED)
find_package(Qt5 REQUIRED Core)

add_library(app_core OBJECT
    entities/Application.cpp
    entities/Application.h
    entities/MetadataExtractor.cpp
    entities/MetadataExtractor.h
    entities/DesktopFileMetadataExtractor.cpp
    entities/DesktopFileMetadataExtractor.h
    entities/AppStreamMetadataExtractor.cpp
    entities/AppStreamMetadataExtractor.h)


target_compile_options(app_core PUBLIC -fPIC -fexceptions)
target_include_directories(app_core PUBLIC
    ${APPIMAGEKIT_INCLUDE_DIRECTORIES}
    ${Qt5Core_INCLUDE_DIRS} )


add_executable(appimage_releases_indexer main.cpp $<TARGET_OBJECTS:app_core>)

target_link_libraries(appimage_releases_indexer ${APPIMAGEKIT_LIBRARIES} Qt5::Core)