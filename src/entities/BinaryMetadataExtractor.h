//
// Created by alexis on 6/8/18.
//

#ifndef APPIMAGE_RELEASES_INDEXER_BINARYMETADATAEXTRACTOR_H
#define APPIMAGE_RELEASES_INDEXER_BINARYMETADATAEXTRACTOR_H

#include <bfd.h>
#include <nlohmann/json.hpp>

class BadFileFormat : public std::runtime_error {
public:
    explicit BadFileFormat(const std::string &__arg);
};

class BinaryMetadataExtractor {
    std::string target;
    bfd *abfd;
public:
    explicit BinaryMetadataExtractor(const std::string &target);

    virtual ~BinaryMetadataExtractor();

    nlohmann::json getMetadata();

protected:
    std::string getSha512CheckSum() const;

    std::string getBinaryArch() const;

    int64_t getFileSize() const;

    int getAppImageType() const;

    time_t getTime() const;
};

#endif //APPIMAGE_RELEASES_INDEXER_BINARYMETADATAEXTRACTOR_H
