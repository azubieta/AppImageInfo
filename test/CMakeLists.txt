add_executable(test_appstreamMetadatadExtractor
        testAppstreamMetadataExtractor.cpp
        ${CMAKE_SOURCE_DIR}/src/entities/AppStreamMetadataExtractor.h
        ${CMAKE_SOURCE_DIR}/src/entities/AppStreamMetadataExtractor.cpp)

target_link_libraries(test_appstreamMetadatadExtractor
        ${LIBAPPIMAGE_LIBRARIES}
        ${LIBBFD_LIBRARIES}
        ${OPENSSL_LIBRARIES}
        ${LIBXML2_LIBRARIES}
        ${Boost_LIBRARIES}
        gtest)
target_include_directories(test_appstreamMetadatadExtractor PRIVATE ${CMAKE_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/include ${LIBAPPIMAGE_INCLUDE_DIRS} ${LIBXML2_INCLUDE_DIR})
target_compile_definitions(test_appstreamMetadatadExtractor PRIVATE TEST_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data" ${LIBXML2_DEFINITIONS})

add_dependencies(test_appstreamMetadatadExtractor libappimage)
add_test(AppstreamMetadatadExtractorTest test_appstreamMetadatadExtractor)

add_executable(test_fileMetadatadExtractor
        testFileMetadaExtractor.cpp
        ${CMAKE_SOURCE_DIR}/src/entities/FileMetadataMerger.cpp
        ${CMAKE_SOURCE_DIR}/src/entities/FileMetadataExtractor.cpp
        ${CMAKE_SOURCE_DIR}/src/entities/BinaryMetadataExtractor.cpp
        ${CMAKE_SOURCE_DIR}/src/entities/AppStreamMetadataExtractor.cpp
        ${CMAKE_SOURCE_DIR}/src/entities/DesktopFileMetadataExtractor.cpp)


target_link_libraries(test_fileMetadatadExtractor
        ${LIBAPPIMAGE_LIBRARIES}
        ${LIBBFD_LIBRARIES}
        ${OPENSSL_LIBRARIES}
        ${LIBXML2_LIBRARIES}
        ${Boost_LIBRARIES}
        gtest)
target_include_directories(test_fileMetadatadExtractor PRIVATE ${CMAKE_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/include ${LIBAPPIMAGE_INCLUDE_DIRS} ${LIBXML2_INCLUDE_DIR})
target_compile_definitions(test_fileMetadatadExtractor PRIVATE TEST_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data" ${LIBXML2_DEFINITIONS})

add_dependencies(test_fileMetadatadExtractor libappimage)
add_test(FileMetadatadExtractorTest test_fileMetadatadExtractor)
