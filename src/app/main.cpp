#include <iostream>
#include <fstream>
#include <boost/program_options.hpp>
#include <nlohmann/json.hpp>

using namespace boost::program_options;


#include "../entities/FileMetadataExtractor.h"

struct {
    std::string AppImagePath;
    std::string AppImageInfoOutputPath;
    std::string AppIconOutputPath;
    bool isOk;
} typedef AppConfig;

void writeAppImageInfo(const nlohmann::json &metadata, const std::string &outputDirPath);


AppConfig parseArguments(int argc, char **argv) {
    AppConfig config;
    config.isOk = false;

    options_description desc{"Options"};
    desc.add_options()
            ("help,h", "Help screen")
            ("appImage", value<std::string>(&config.AppImagePath)->required(), "Target AppImage file")
            ("appImageInfo,o", value<std::string>(&config.AppImageInfoOutputPath), "Output the AppImage info to <appImageInfo>")
            ("appImageIcon,i", value<std::string>(&config.AppIconOutputPath), "Output the AppImage Icon to <appImageIcon>");

    positional_options_description pos_desc;
    pos_desc.add("appImage", 1);

    variables_map vm;
    try {
        command_line_parser parser{argc, argv};
        parser.positional(pos_desc).options(desc);
        parsed_options parsed_options = parser.run();

        store(parsed_options, vm);
        notify(vm);

        config.isOk = true;
    }
    catch (const error &ex) {
        if (vm.find("help") == vm.end())
            std::cerr << ex.what() << std::endl;
        std::cout << desc << std::endl;
    }
    return config;
}

int main(int argc, char **argv) {
    AppConfig config = parseArguments(argc, argv);

    if (config.isOk) {
        FileMetadataExtractor fileMetadataExtractor;
        fileMetadataExtractor.setPath(config.AppImagePath);

        auto metadata = fileMetadataExtractor.extractMetadata();

        if (config.AppImageInfoOutputPath.empty())
            std::cout << metadata.dump(2) << std::endl;
        else {
            std::ofstream f(config.AppImageInfoOutputPath, std::ofstream::out | std::ofstream::app);
            f << metadata.dump(2) << std::endl;
            f.close();
        }

        if (!config.AppIconOutputPath.empty())
            fileMetadataExtractor.extractIconFile(config.AppIconOutputPath.c_str(), "256x256");

        return 0;
    } else
        return 1;
}