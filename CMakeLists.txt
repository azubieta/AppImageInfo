cmake_minimum_required(VERSION 3.8)
project(AppImageInfo
        VERSION 1.1.3
        LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Offer the user the choice of overriding the installation directories
set(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libraries")
set(INSTALL_BIN_DIR bin CACHE PATH "Installation directory for executables")
set(INSTALL_INCLUDE_DIR include CACHE PATH "Installation directory for header files")
set(INSTALL_CMAKE_DIR lib/cmake/AppImageInfo CACHE PATH "Installation directory for CMake files")

add_subdirectory(thirdparty)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty")

find_package(LibXml2 REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Boost COMPONENTS program_options filesystem system REQUIRED)

include(FeatureSummary)
feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)

add_subdirectory(src)

if (NOT TARGET gtest)
    add_subdirectory(thirdparty/googletest EXCLUDE_FROM_ALL)
endif()
add_subdirectory(test EXCLUDE_FROM_ALL)

export(TARGETS libappimageinfo appimageinfotool
        FILE "${PROJECT_BINARY_DIR}/AppImageInfoTargets.cmake")

# Export the package for use from the build-tree
# (this registers the build-tree with a global CMake-registry)
export(PACKAGE AppImageInfo)

# Make relative paths absolute (needed later on)
foreach(p LIB BIN INCLUDE CMAKE)
    set(var INSTALL_${p}_DIR)
    if(NOT IS_ABSOLUTE "${${var}}")
        set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
    endif()
endforeach()

# Create the AppImageInfoConfig.cmake and AppImageInfoConfigVersion files
file(RELATIVE_PATH REL_INCLUDE_DIR "${INSTALL_CMAKE_DIR}"
        "${INSTALL_INCLUDE_DIR}")
# ... for the build tree
set(CONF_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/include" "${PROJECT_BINARY_DIR}/include")
configure_file(AppImageInfoConfig.cmake.in
        "${PROJECT_BINARY_DIR}/AppImageInfoConfig.cmake" @ONLY)
# ... for the install tree
set(CONF_INCLUDE_DIRS "\${APPIMAGEINFO_CMAKE_DIR}/${REL_INCLUDE_DIR}")
configure_file(AppImageInfoConfig.cmake.in
        "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/AppImageInfoConfig.cmake" @ONLY)
# ... for both
configure_file(AppImageInfoConfigVersion.cmake.in
        "${PROJECT_BINARY_DIR}/AppImageInfoConfigVersion.cmake" @ONLY)

# Install the AppImageInfoConfig.cmake and AppImageInfoConfigVersion.cmake
install(FILES
        "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/AppImageInfoConfig.cmake"
        "${PROJECT_BINARY_DIR}/AppImageInfoConfigVersion.cmake"
        DESTINATION "${INSTALL_CMAKE_DIR}" COMPONENT dev)

# Install the export set for use with the install-tree
install(EXPORT AppImageInfoTargets DESTINATION
        "${INSTALL_CMAKE_DIR}" COMPONENT dev)
