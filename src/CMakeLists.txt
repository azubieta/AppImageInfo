find_package(Qt5 REQUIRED Core)

set(CMAKE_AUTOMOC ON)

add_library(app_core OBJECT
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
        )


target_compile_options(app_core PUBLIC -fPIC -fexceptions)
target_include_directories(app_core PUBLIC
        ${APPIMAGEKIT_INCLUDE_DIRS}
        ${Qt5Core_INCLUDE_DIRS}
        ${Qt5Gui_INCLUDE_DIRS})

if (NOT AppImageKit_FOUND)
    add_dependencies(app_core AppImageKit)
endif ()

add_executable(appimage_metadata_extractor app/main.cpp $<TARGET_OBJECTS:app_core>)
target_link_libraries(appimage_metadata_extractor ${APPIMAGEKIT_LIBRARIES} Qt5::Core)
target_compile_definitions(appimage_metadata_extractor PRIVATE PROJECT_NAME="${PROJECT_NAME}" PRIVATE PROJECT_VERSION="${PROJECT_VERSION}")

install(TARGETS appimage_metadata_extractor DESTINATION bin COMPONENT binaries)
install(FILES app/res/appimage_metadata_extractor.desktop DESTINATION share/applications/ COMPONENT desktop)
install(FILES app/res/appimage_metadata_extractor.svg DESTINATION share/icons/hicolor/scalable/apps/ COMPONENT desktop)