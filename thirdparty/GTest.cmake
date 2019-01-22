message(STATUS "Downloading and building GTest")

externalproject_add(GTest_External
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.8.0
    UPDATE_COMMAND ""  # make sure CMake won't try to updateRepository updates unnecessarily and hence rebuild the dependency every time
    INSTALL_COMMAND ""
    CONFIGURE_COMMAND ${CMAKE_COMMAND} -G${CMAKE_GENERATOR} -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR> <SOURCE_DIR>/googletest
    )

externalproject_get_property(GTest_External SOURCE_DIR)
externalproject_get_property(GTest_External BINARY_DIR)

add_library(gtest STATIC IMPORTED)
set_target_properties(
    gtest PROPERTIES
    IMPORTED_LOCATION ${BINARY_DIR}/libgtest.a;
    INTERFACE_INCLUDE_DIRECTORIES ${SOURCE_DIR}/googletest/include/
    INTERFACE_LINK_LIBRARIES "pthread"
)
add_dependencies(gtest GTest_External)


add_library(gtest_main STATIC IMPORTED)
set_target_properties(
    gtest_main PROPERTIES
    IMPORTED_LOCATION ${BINARY_DIR}/libgtest_main.a;
    INTERFACE_INCLUDE_DIRECTORIES ${SOURCE_DIR}/googletest/include/
    INTERFACE_LINK_LIBRARIES "pthread"
)
add_dependencies(gtest_main GTest_External)
