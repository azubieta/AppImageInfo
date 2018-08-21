#include <iostream>
#include <fstream>
#include <boost/program_options.hpp>

using namespace boost::program_options;


#include "../entities/FileMetadataExtractor.h"

struct {
    std::string AppImagePath;
    std::string AppImageInfoOutputPath;
    std::string AppIconOutputPath;
} typedef AppConfig;

void writeAppImageInfo(const nlohmann::json &metadata, const std::string &outputDirPath);


AppConfig parseArguments(int argc, char **argv) {
    AppConfig config{};
    try {
        options_description desc{"Options"};
        desc.add_options()
                ("help,h", "Help screen")
                ("appImage", value<std::string>(&config.AppImagePath)->required(), "Target AppImage file")
                ("appImageInfo,o", value<std::string>(), "Output the AppImage info to <appImageInfo>")
                ("appImageIcon,i", value<std::string>(), "Output the AppImage Icon to <appImageIcon>");

        positional_options_description pos_desc;
        pos_desc.add("appImage", 1);

        command_line_parser parser{argc, argv};
        parser.positional(pos_desc).options(desc);
        parsed_options parsed_options = parser.run();

        variables_map vm;
        store(parsed_options, vm);
        notify(vm);

        if (!vm.count("appImage")) {
            std::cout << "Missing AppImage file." << std::endl;
            std::cout << desc << '\n';
            exit(1);
        }

        if (vm.count("help")) {
            std::cout << desc << '\n';
            exit(0);
        } else {
            if (vm.count("appImage"))
                config.AppImagePath = vm["appImage"].as<std::string>();

            if (vm.count("appImageInfo"))
                config.AppImageInfoOutputPath = vm["appImageInfo"].as<std::string>();

            if (vm.count("appImageIcon"))
                config.AppIconOutputPath = vm["appImageIcon"].as<std::string>();
        }
    }
    catch (const error &ex) {
        std::cerr << ex.what() << '\n';
    }
    return config;
}

int main(int argc, char **argv) {
    AppConfig config = parseArguments(argc, argv);

    FileMetadataExtractor fileMetadataExtractor;
    fileMetadataExtractor.setPath(config.AppImagePath);

    auto metadata = fileMetadataExtractor.extractMetadata();

    if (config.AppImageInfoOutputPath.empty())
        std::cout << metadata.dump(2, ' ', true) << std::endl;
    else {
        std::ofstream f(config.AppImageInfoOutputPath, std::ofstream::out | std::ofstream::app);
        f << metadata.dump(2, ' ', true) << std::endl;
        f.close();
    }

    if (!config.AppIconOutputPath.empty())
        fileMetadataExtractor.extractIconFile(config.AppIconOutputPath.c_str(), "256x256");

    return 0;
}