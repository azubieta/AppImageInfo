cmake_minimum_required(VERSION 3.8)
project(appimage_metadata_extractor VERSION 0.1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(thirdparty/AppImageKit.cmake)

add_subdirectory(src)
add_subdirectory(test)
