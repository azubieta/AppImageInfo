find_package(Qt5 REQUIRED Core Test)

set(CMAKE_AUTOMOC ON)
add_executable(test_metadata_extrator testMetadataExtractor.cpp)

target_include_directories(test_metadata_extrator PRIVATE ${CMAKE_SOURCE_DIR}/src ${APPIMAGEKIT_INCLUDE_DIRS})
target_link_libraries(test_metadata_extrator libappimage-info Qt5::Core Qt5::Test ${APPIMAGEKIT_LIBRARIES} ${LIBBFD_LIBRARIES} ${LIBMD_LIBRARIES})
target_compile_definitions(test_metadata_extrator PRIVATE TEST_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data")

add_executable(test_appstreamMetadatadExtractor
        testAppstreamMetadataExtractor.cpp
        ${CMAKE_SOURCE_DIR}/src/entities/AppStreamMetadataExtractor.h
        ${CMAKE_SOURCE_DIR}/src/entities/AppStreamMetadataExtractor.cpp testFileMetadaExtractor.cpp)

target_link_libraries(test_appstreamMetadatadExtractor
        ${LIBXML2_LIBRARIES}
        gtest)
target_include_directories(test_appstreamMetadatadExtractor PRIVATE ${CMAKE_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/include ${LIBXML2_INCLUDE_DIR})
target_compile_definitions(test_appstreamMetadatadExtractor PRIVATE TEST_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data" ${LIBXML2_DEFINITIONS})

add_test(AppstreamMetadatadExtractorTest test_appstreamMetadatadExtractor)

add_executable(test_fileMetadatadExtractor
        testFileMetadaExtractor.cpp
        ${CMAKE_SOURCE_DIR}/src/entities/FileMetadataMerger.cpp
        ${CMAKE_SOURCE_DIR}/src/entities/FileMetadataExtractor.cpp
        ${CMAKE_SOURCE_DIR}/src/entities/BinaryMetadataExtractor.cpp
        ${CMAKE_SOURCE_DIR}/src/entities/AppStreamMetadataExtractor.cpp
        ${CMAKE_SOURCE_DIR}/src/entities/DesktopFileMetadataExtractor.cpp)

message("BOOST LOG: " ${Boost_LOG_LIBRARY})

target_link_libraries(test_fileMetadatadExtractor
        ${APPIMAGEKIT_LIBRARIES}
        ${LIBBFD_LIBRARIES}
        ${LIBMD_LIBRARIES}
        ${LIBXML2_LIBRARIES}
        ${Boost_LIBRARIES}
        gtest)
target_include_directories(test_fileMetadatadExtractor PRIVATE ${CMAKE_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/include ${LIBXML2_INCLUDE_DIR})
target_compile_definitions(test_fileMetadatadExtractor PRIVATE TEST_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data" ${LIBXML2_DEFINITIONS})

add_test(AppstreamMetadatadExtractorTest test_appstreamMetadatadExtractor)