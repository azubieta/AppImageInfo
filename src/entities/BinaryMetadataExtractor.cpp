//
// Created by alexis on 6/8/18.
//

extern "C" {
#include <sys/stat.h>
}

#include <fstream>
#include <appimage/appimage.h>
#include <openssl/sha.h>
#include <nlohmann/json.hpp>
#include "BinaryMetadataExtractor.h"

BinaryMetadataExtractor::BinaryMetadataExtractor(const std::string &target)
        : target(target), abfd(nullptr) {
    abfd = bfd_openr(target.c_str(), nullptr);

    // no section info is loaded unless we call bfd_check_format!:
    if (!bfd_check_format(abfd, bfd_object))
        throw BadFileFormat(bfd_errmsg(bfd_get_error()));
}

nlohmann::json BinaryMetadataExtractor::getMetadata() {
    nlohmann::json data;
    data["architecture"] = getBinaryArch();
    data["sha512checksum"] = getSha512CheckSum();
    data["size"] = getFileSize();
    data["type"] = getAppImageType();
    data["date"] = getTime();

    return data;
}

time_t BinaryMetadataExtractor::getTime() const {
    auto date = bfd_get_mtime(abfd);
    return static_cast<time_t>(date);;
}

int64_t BinaryMetadataExtractor::getFileSize() const {
    struct stat statbuf{};

    if (stat("file.dat", &statbuf) == -1) {
        /* check the value of errno */
    }


    return statbuf.st_size;
}

std::string BinaryMetadataExtractor::getSha512CheckSum() const {
    FILE *file = fopen(target.c_str(), "r");
    if (file) {
        SHA512_CTX sha512;
        SHA512_Init(&sha512);
        uint8_t buffer[1024];

        auto bc = fread(buffer, sizeof(uint8_t), 1024, file);
        while (bc) {
            SHA512_Update(&sha512, buffer, static_cast<size_t>(bc));
            bc = fread(buffer, sizeof(uint8_t), 1024, file);
        }

        unsigned char hash[SHA512_DIGEST_LENGTH];
        SHA512_Final(hash, &sha512);

        char mdString[SHA512_DIGEST_LENGTH * 2 + 1];
        for (int i = 0; i < SHA512_DIGEST_LENGTH; i++)
            sprintf(&mdString[i * 2], "%02x", (unsigned int) hash[i]);

        std::string out = mdString;
        return out;
    }

    return "";
}

std::string BinaryMetadataExtractor::getBinaryArch() const {
    std::string arch = abfd->arch_info->arch_name;
    return arch;
}

int BinaryMetadataExtractor::getAppImageType() const {
    return appimage_get_type(target.c_str(), false);
}

BinaryMetadataExtractor::~BinaryMetadataExtractor() {
    bfd_close(abfd);
}

BadFileFormat::BadFileFormat(const std::string &__arg) : runtime_error(__arg) {}
