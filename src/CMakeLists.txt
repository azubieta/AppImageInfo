find_package(AppImageKit REQUIRED)
find_package(Qt5 REQUIRED Core Network)

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
        gateways/PageDownloader.cpp
        gateways/PageDownloader.h
        entities/GitHubProjectIndexer.cpp
        entities/GitHubProjectIndexer.h
        gateways/FileDownloader.cpp
        gateways/FileDownloader.h
        gateways/GitHubApiClient.cpp
        gateways/GitHubApiClient.h
        entities/RemoteAppImageMetadataExtractor.cpp
        entities/RemoteAppImageMetadataExtractor.h
        entities/AppInfoBuilder.h
        entities/AppInfoBuilder.cpp)


target_compile_options(app_core PUBLIC -fPIC -fexceptions)
target_include_directories(app_core PUBLIC
        ${APPIMAGEKIT_INCLUDE_DIRECTORIES}
        ${Qt5Core_INCLUDE_DIRS}
        ${Qt5Network_INCLUDE_DIRS})

add_executable(appimage_indexer app/indexer_main.cpp app/Indexer.cpp app/Indexer.h $<TARGET_OBJECTS:app_core>)
target_link_libraries(appimage_indexer ${APPIMAGEKIT_LIBRARIES} Qt5::Core Qt5::Network)
target_compile_definitions(appimage_indexer PRIVATE PROJECT_NAME="${PROJECT_NAME}" PRIVATE PROJECT_VERSION="${PROJECT_VERSION}")

install(TARGETS appimage_indexer DESTINATION bin COMPONENT binaries)
install(FILES app/res/appimage-indexer.desktop DESTINATION share/applications/ COMPONENT desktop)
install(FILES app/res/appimage-indexer.svg DESTINATION share/icons/hicolor/scalable/apps/ COMPONENT desktop)