find_package(Qt5 REQUIRED Core Network Test)

set(CMAKE_AUTOMOC ON)

add_executable(test_metadata_extrator testMetadataExtractor.cpp $<TARGET_OBJECTS:app_core>)

target_include_directories(test_metadata_extrator PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_metadata_extrator Qt5::Core Qt5::Network Qt5::Test appimage)
target_compile_definitions(test_metadata_extrator PRIVATE TEST_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data")

add_executable(test_downloader testDownloader.cpp $<TARGET_OBJECTS:app_core>)

target_include_directories(test_downloader PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_downloader Qt5::Core Qt5::Network Qt5::Test appimage)
target_compile_definitions(test_downloader PRIVATE TEST_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data")


