find_package(Qt5 REQUIRED Core Test)

set(CMAKE_AUTOMOC ON)
add_executable(test_metadata_extrator testMetadataExtractor.cpp)

target_include_directories(test_metadata_extrator PRIVATE ${CMAKE_SOURCE_DIR}/src ${APPIMAGEKIT_INCLUDE_DIRS})
target_link_libraries(test_metadata_extrator libappimage-info Qt5::Core Qt5::Test ${APPIMAGEKIT_LIBRARIES})
target_compile_definitions(test_metadata_extrator PRIVATE TEST_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data")

