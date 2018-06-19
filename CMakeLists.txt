cmake_minimum_required(VERSION 3.8)
project(appimage_releases_indexer VERSION 0.1)

set(CMAKE_C_STANDARD 17)

include(thirdparty/AppImageKit.cmake)
install(FILES ${libappimage_PATH} DESTINATION lib COMPONENT libappimage)

add_subdirectory(src)
add_subdirectory(test)
