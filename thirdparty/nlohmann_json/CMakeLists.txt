add_library(nlohmann_json INTERFACE)

target_include_directories(nlohmann_json INTERFACE "${PROJECT_SOURCE_DIR}/thirdparty/nlohmann_json/include")
target_sources(nlohmann_json INTERFACE "${PROJECT_SOURCE_DIR}/thirdparty/nlohmann_json/include/nlohmann/json.hpp")
