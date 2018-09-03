enable_testing()
add_executable(test_desktopFileMetadatadExtractor
        testDesktopFileMetadataExtractor.cpp
        ${PROJECT_SOURCE_DIR}/src/entities/DesktopFileMetadataExtractor.h
        ${PROJECT_SOURCE_DIR}/src/entities/DesktopFileMetadataExtractor.cpp )

target_link_libraries(test_desktopFileMetadatadExtractor
        ${LIBAPPIMAGE_LIBRARIES}
        ${LIBBFD_LIBRARIES}
        ${OPENSSL_LIBRARIES}
        ${LIBXML2_LIBRARIES}
        ${Boost_LIBRARIES}
        gtest)
target_include_directories(test_desktopFileMetadatadExtractor PRIVATE
        ${PROJECT_SOURCE_DIR}/src
        ${NLOHMANN_JSON_INCLUDE_DIRS})

target_compile_definitions(test_desktopFileMetadatadExtractor PRIVATE TEST_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data" ${LIBXML2_DEFINITIONS})

add_test(AppstreamMetadatadExtractorTest test_appstreamMetadatadExtractor)


add_executable(test_appstreamMetadatadExtractor
        testAppstreamMetadataExtractor.cpp
        ${PROJECT_SOURCE_DIR}/src/entities/AppStreamMetadataExtractor.h
        ${PROJECT_SOURCE_DIR}/src/entities/AppStreamMetadataExtractor.cpp)

target_link_libraries(test_appstreamMetadatadExtractor
        ${LIBAPPIMAGE_LIBRARIES}
        ${LIBBFD_LIBRARIES}
        ${OPENSSL_LIBRARIES}
        ${LIBXML2_LIBRARIES}
        ${Boost_LIBRARIES}
        gtest)
target_include_directories(test_appstreamMetadatadExtractor PRIVATE
        ${PROJECT_SOURCE_DIR}}/src ${PROJECT_SOURCE_DIR}/include
        ${LIBAPPIMAGE_INCLUDE_DIRS}
        ${LIBXML2_INCLUDE_DIR}
        ${NLOHMANN_JSON_INCLUDE_DIRS})

target_compile_definitions(test_appstreamMetadatadExtractor PRIVATE TEST_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data" ${LIBXML2_DEFINITIONS})

add_dependencies(test_appstreamMetadatadExtractor libappimage)
add_test(AppstreamMetadatadExtractorTest test_appstreamMetadatadExtractor)

add_executable(test_fileMetadatadExtractor
        testFileMetadaExtractor.cpp
        ${PROJECT_SOURCE_DIR}/src/entities/FileMetadataMerger.cpp
        ${PROJECT_SOURCE_DIR}/src/entities/FileMetadataExtractor.cpp
        ${PROJECT_SOURCE_DIR}/src/entities/BinaryMetadataExtractor.cpp
        ${PROJECT_SOURCE_DIR}/src/entities/AppStreamMetadataExtractor.cpp
        ${PROJECT_SOURCE_DIR}/src/entities/DesktopFileMetadataExtractor.cpp)

target_include_directories(test_fileMetadatadExtractor PRIVATE
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/include
        ${LIBAPPIMAGE_INCLUDE_DIRS}
        ${LIBXML2_INCLUDE_DIR}
        ${NLOHMANN_JSON_INCLUDE_DIRS})

target_link_libraries(test_fileMetadatadExtractor
        ${LIBAPPIMAGE_LIBRARIES}
        ${LIBBFD_LIBRARIES}
        ${OPENSSL_LIBRARIES}
        ${LIBXML2_LIBRARIES}
        ${Boost_LIBRARIES}
        gtest)


target_compile_definitions(test_fileMetadatadExtractor PRIVATE TEST_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data" ${LIBXML2_DEFINITIONS})

add_dependencies(test_fileMetadatadExtractor libappimage)
add_test(FileMetadatadExtractorTest test_fileMetadatadExtractor)
