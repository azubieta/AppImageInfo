include(LibAppImage.cmake)

add_target(json INTERFACE)
set_property(TARGET json INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/nlohmann_json/include")
target_sources(json "${CMAKE_CURRENT_SOURCE_DIR}/nlohmann_json/include/json.h")
