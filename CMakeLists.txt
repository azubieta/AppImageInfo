cmake_minimum_required(VERSION 3.8)
project(appimage_info VERSION 0.2)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(thirdparty)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty")

find_package(Bfd REQUIRED)
find_package(LibXml2 REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Boost COMPONENTS program_options filesystem system REQUIRED)

set(LIBAPPIMAGE-INFO_VERSION_MAJOR 0)
set(LIBAPPIMAGE-INFO_VERSION_MINOR 1)
set(LIBAPPIMAGE-INFO_VERSION_PATCH 1)
set(LIBAPPIMAGE-INFO_VERSION ${LIBAPPIMAGE-INFO_VERSION_MAJOR}.${LIBAPPIMAGE-INFO_VERSION_MINOR}.${LIBAPPIMAGE-INFO_VERSION_PATCH} )


add_subdirectory(src)

add_subdirectory(thirdparty/googletest EXCLUDE_FROM_ALL)
add_subdirectory(test EXCLUDE_FROM_ALL)
