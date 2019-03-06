#pragma once

#include <nlohmann/json.hpp>
#include <boost/filesystem/path.hpp>


class FileMetadataExtractor {
    std::string path{};
    std::list<std::string> list{};
public:
    FileMetadataExtractor();

    explicit FileMetadataExtractor(const std::string& path);

    // Throws runtime_error when isn't possible to get the files list;
    std::list<std::string> loadFileList();

    const std::string& getPath() const;

    void setPath(const std::string& path);

    nlohmann::json extractMetadata();

    std::string getDesktopFileName(const std::list<std::string>& list) const;

    void extractDesktopFile(const std::string& outputPath);

    void extractAppStreamFile(const char* targetPath);

    void extractIconFile(const char* targetPath, const char* iconSize);

protected:
    nlohmann::json extractDesktopFileData();

    void tryExtractFile(const boost::filesystem::path& filePath, boost::filesystem::path targetDir) const;

    nlohmann::json extractAppStreamFileData();

    nlohmann::json extractBinaryFileData();

    std::string tryGetDesktopFileName(const std::list<std::string>& list) const;

    boost::filesystem::path getTmpDir() const;

    boost::filesystem::path tryGetAppStreamFileName(std::list<std::string> list);
};
