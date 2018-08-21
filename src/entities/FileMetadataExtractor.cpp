//
// Created by alexis on 6/5/18.
//

#include <appimage/appimage.h>
#include <boost/filesystem.hpp>
#include <iostream>

#include "FileMetadataExtractor.h"
#include "DesktopFileMetadataExtractor.h"
#include "AppStreamMetadataExtractor.h"
#include "BinaryMetadataExtractor.h"
#include "FileMetadataMerger.h"

using namespace boost;

FileMetadataExtractor::FileMetadataExtractor(const std::string &path)
        : path(path) {}

std::list<std::string> FileMetadataExtractor::loadFileList() {
    std::list<std::string> fileList;
    char **rawFileList = appimage_list_files(path.c_str());
    if (rawFileList == nullptr || *rawFileList == nullptr)
        throw std::runtime_error(std::string("Unable to list files in ") + path);

    for (int i = 0; rawFileList[i] != nullptr; i++)
        fileList.push_back(std::string(rawFileList[i]));

    appimage_string_list_free(rawFileList);
    return fileList;
}

nlohmann::json FileMetadataExtractor::extractDesktopFileData() {
    auto tmpFilePath =
            filesystem::temp_directory_path() / filesystem::unique_path("appimage-info-%%%%%%%%%%%%.desktop");

    auto desktopFileName = tryGetDesktopFileName(list);
    tryExtractFile(desktopFileName, tmpFilePath);

    DesktopFileMetadataExtractor extractor(tmpFilePath.string());
    auto data = extractor.getContent();

    filesystem::remove(tmpFilePath);

    return data;
}

boost::filesystem::path FileMetadataExtractor::getTmpDir() const {
    filesystem::path tmpDir{"/tmp/appimage_indexer"};
    if (!filesystem::exists(tmpDir))
        create_directories(tmpDir);

    return tmpDir;
}

std::string FileMetadataExtractor::getDesktopFileName(const std::list<std::string> &list) const {
    std::string desktopfileName;

    for (const auto &file : list) {
        if (file.find('/') == std::string::npos && file.find(".desktop") != std::string::npos)
            desktopfileName = file;
    }

    return desktopfileName;
}

std::string FileMetadataExtractor::tryGetDesktopFileName(const std::list<std::string> &list) const {
    auto fileName = getDesktopFileName(list);
    if (fileName.empty())
        throw std::runtime_error(std::string("Not .desktop file found."));

    return fileName;
}

void FileMetadataExtractor::tryExtractFile(const filesystem::path &filePath, boost::filesystem::path targetPath) const {
    appimage_extract_file_following_symlinks(path.c_str(), filePath.c_str(), targetPath.c_str());

    if (!filesystem::exists(targetPath))
        throw std::runtime_error("Failed to extract file: " + filePath.string());
}

nlohmann::json FileMetadataExtractor::extractAppStreamFileData() {
    auto tmpFilePath =
            filesystem::temp_directory_path() / filesystem::unique_path("appimage-info-appstream-%%%%%%%%.xml");

    auto appStreamFileName = tryGetAppStreamFileName(list);
    tryExtractFile(appStreamFileName, tmpFilePath);
    AppStreamMetadataExtractor extractor(tmpFilePath.string());
    auto content = extractor.getContent();

    filesystem::remove(tmpFilePath);
    return content;
}

filesystem::path FileMetadataExtractor::tryGetAppStreamFileName(std::list<std::string> list) {
    filesystem::path fileName;

    for (auto file : list)
        if (file.find("usr/share/metainfo") != file.npos && file.find(".appdata.xml") != file.npos) {
            fileName = file;
            break;
        }

    if (fileName.empty())
        throw std::runtime_error(std::string("Not .appdata.xml file found.\n"));

    return fileName;
}

FileMetadataExtractor::FileMetadataExtractor() {

}

const std::string &FileMetadataExtractor::getPath() const {
    return path;
}

void FileMetadataExtractor::setPath(const std::string &path) {
    FileMetadataExtractor::path = path;
}

nlohmann::json FileMetadataExtractor::extractMetadata() {
    list = loadFileList();
    nlohmann::json desktop;
    nlohmann::json appStream;
    nlohmann::json binary;
    try {
        desktop = extractDesktopFileData();
    }
    catch (std::runtime_error &e) {
        std::cerr << e.what();
    }

    try {
        appStream = extractAppStreamFileData();
    }
    catch (std::runtime_error &e) {
        std::cerr << e.what();
    }

    try {
        binary = extractBinaryFileData();
    }
    catch (std::runtime_error &e) {
        std::cerr << e.what();
    }

    MetadataMerger merger;
    merger.setAppStream(appStream);
    merger.setDesktop(desktop);
    merger.setBinary(binary);

    return merger.getOutput();
}

nlohmann::json FileMetadataExtractor::extractBinaryFileData() {
    BinaryMetadataExtractor extractor(path);
    return extractor.getMetadata();
}

void FileMetadataExtractor::extractDesktopFile(const std::string &outputPath) {
    auto desktopFileName = tryGetDesktopFileName(list);
    appimage_extract_file_following_symlinks(path.c_str(), desktopFileName.c_str(), outputPath.c_str());
}

void FileMetadataExtractor::extractAppStreamFile(const char *targetPath) {
    auto appstreamFileName = tryGetAppStreamFileName(list);
    appimage_extract_file_following_symlinks(path.c_str(), appstreamFileName.c_str(), targetPath);
}

void FileMetadataExtractor::extractIconFile(const char *targetPath, const char *iconSize) {
    // Search for an icon of the required size in
    for (const auto &file: list) {
        if (file.find("/usr/share/icons/hicolor") == std::string::npos && file.find(iconSize) != std::string::npos) {
            appimage_extract_file_following_symlinks(path.c_str(), file.c_str(), targetPath);
            return;
        }
    }

    // Fallback to the .AppIcon
    appimage_extract_file_following_symlinks(path.c_str(), ".DirIcon", targetPath);
}
