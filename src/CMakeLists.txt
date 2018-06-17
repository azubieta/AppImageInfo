find_package(AppImageKit REQUIRED)
find_package(Qt5 REQUIRED Core Network)

set(CMAKE_AUTOMOC ON)

add_library(app_core OBJECT
    entities/Application.cpp
    entities/Application.h
    entities/MetadataExtractor.cpp
    entities/MetadataExtractor.h
    entities/DesktopFileMetadataExtractor.cpp
    entities/DesktopFileMetadataExtractor.h
    entities/AppStreamMetadataExtractor.cpp
    entities/AppStreamMetadataExtractor.h
    entities/MetadataMerger.cpp
    entities/MetadataMerger.h
    entities/BinaryMetadataExtractor.cpp
    entities/BinaryMetadataExtractor.h
    gateways/PageDownloader.cpp
    gateways/PageDownloader.h entities/Indexer.cpp entities/Indexer.h entities/GitHubProjectIndexer.cpp entities/GitHubProjectIndexer.h gateways/FileDownloader.cpp gateways/FileDownloader.h gateways/GitHubApiClient.cpp gateways/GitHubApiClient.h entities/RemoteAppImageMetadataExtractor.cpp entities/RemoteAppImageMetadataExtractor.h)


target_compile_options(app_core PUBLIC -fPIC -fexceptions)
target_include_directories(app_core PUBLIC
    ${APPIMAGEKIT_INCLUDE_DIRECTORIES}
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Network_INCLUDE_DIRS})


add_executable(appimage_metadata_extractor main.cpp $<TARGET_OBJECTS:app_core>)

target_link_libraries(appimage_metadata_extractor ${APPIMAGEKIT_LIBRARIES} Qt5::Core Qt5::Network)

target_compile_definitions(appimage_metadata_extractor PRIVATE PROJECT_NAME="${PROJECT_NAME}" PRIVATE PROJECT_VERSION="${PROJECT_VERSION}")